#include "Terrain.h"
#include "Chunk/Chunk.h"
#include "utils/sort.h"
#include <iostream>

 Terrain::Terrain(Player* player)
 {
	 m_CurrentChunk = 0;

	 int chunkX = player->GetChunkGridPosition().x;
	 int chunkZ = player->GetChunkGridPosition().z;

	 unsigned int chunk_offset = 0; // chunk's offset into the buffers allocated by the terrain
	 for (int z = chunkZ - CHUNK_RADIUS; z <= chunkZ + CHUNK_RADIUS; z++)
	 {
		 for (int x = chunkX - CHUNK_RADIUS; x <= chunkX + CHUNK_RADIUS; x++)
		 {
			 Chunk* chunk = new Chunk(vec3(x, 0, z), player, chunk_offset);

			 m_Chunks.push_back(chunk);

			 chunk_offset++;
		 }
	 }

	 // allocate buffers for model matrices, texture coordinates and faces indexe
	 // model buffer: allocate 16 float values for every face of every block, that's a lot more than the matrices that will be calculated in the mesh
	 unsigned int num_of_faces = m_Chunks[0]->GetBlocksVector().size() * FACES_PER_BLOCK;
	 unsigned int num_of_chunks = m_Chunks.size();

	 unsigned int modelBufSize = 16 * sizeof(float) * num_of_faces * num_of_chunks; // almost 2MB of data
	 unsigned int texCoordsBufSize = 2 * sizeof(float) * INDICES_PER_FACE * num_of_faces * num_of_chunks;
	 unsigned int facesIndexBufSize = sizeof(int) * num_of_faces * num_of_chunks;

	 m_Buffers = (terrain_buffers*)malloc(sizeof(terrain_buffers));

	 if (m_Buffers == NULL)
	 {
		 printf("terrain buffers not allocated");
		 return;
	 }

	 m_Buffers->model = new Buffer((float*)NULL, modelBufSize);
	 m_Buffers->tex = new Buffer((float*)NULL, texCoordsBufSize);
	 m_Buffers->face_index = new Buffer((float*)NULL, facesIndexBufSize);

	 // create the meshes
	 for (int i = 0; i < m_Chunks.size(); i++)
	 {
		 Chunk* c = m_Chunks[i];
		 Mesh* mesh = c->GetMesh();

		 SetChunkSurroundings(c, player->GetChunkGridPosition(), i);

		 mesh->Build(m_Buffers);
	 }
 }

 Terrain::~Terrain()
 {
	 delete m_Buffers->model;
	 delete m_Buffers->tex;
	 delete m_Buffers->face_index;

	 free(m_Buffers);

	 for (Chunk* chunk : m_Chunks)
	 {
		 delete chunk;
	 }

	 m_Chunks.clear();
 }

/*
void Terrain::GenerateWorld(Player* player)
{
	// playerChunk.y doesn't change, the chunks are only created on the xz plane
	vec3 playerChunk = player->GetChunkGridPosition();

	vec3 lastChunkGridPosition = player->GetLastChunkGridPosition();

	if(playerChunk.x == lastChunkGridPosition.x && playerChunk.z == lastChunkGridPosition.z && m_Chunks.size() > 0)
		return;

	// delta between player's last and current position
	vec3 deltaPos = player->GetChunkGridPosition() - player->GetLastChunkGridPosition();

	// replace the out-of-range-from-the-player chunk's positions with new positions in the range
	rearrange_chunks(m_Chunks, playerChunk, deltaPos);

	// sort the chunks by z and x coords to properly set their surrounding chunks
	insertion_sort_chunks(m_Chunks, 0, m_Chunks.size() - 1, Z_COORD);

	int chunkRowLength = CHUNK_RADIUS * 2 + 1;
	
	for (int i = 0; i < m_Chunks.size() / chunkRowLength; i++)
	{
		int start = chunkRowLength * i;
		int end = start + chunkRowLength - 1;

		insertion_sort_chunks(m_Chunks, start, end, X_COORD);
	}

	// create the meshes for every chunk
	for(int i = 0; i < m_Chunks.size(); i++)
	{
		Chunk* c = m_Chunks[i];

		SetChunkSurroundings(c, playerChunk, i);

		c->CreateMesh(player);
	}
}
*/


 void Terrain::GenerateWorld(Player* player)
 {
	 int chunkX = player->GetChunkGridPosition().x;
	 int chunkZ = player->GetChunkGridPosition().z;

	 vec3 lastChunkGridPosition = player->GetLastChunkGridPosition();

	 // if the player hasn't moved between chunks then no need to check the world boundaries
	 // unless the world has not been created yet (when m_Chunks.size())
	 if (chunkX == lastChunkGridPosition.x && chunkZ == lastChunkGridPosition.z && m_Chunks.size() > 0)
	 {
		 //// create the meshes, if camera moved or rotated they will be different, very slow
		 //for (int i = 0; i < m_Chunks.size(); i++)
		 //{
			// Chunk* c = m_Chunks[i];
			// Mesh* mesh = c->GetMesh();

			// SetChunkSurroundings(c, player->GetChunkGridPosition(), i);

			// mesh->Build(m_Buffers);
		 //}

		 // if the player doesn't change its positiom in the grid, keep generating the meshes that haven't beem yet
		 // until the last one is generated

		 if (m_CurrentChunk + 1 < m_Chunks.size())
			 GenerateMeshes(m_Chunks[++m_CurrentChunk], m_CurrentChunk);

		 return;
	 }

	 // CHUNK_RADIUS * deltaPos: if deltaPos.coordinate > 0 i chunk vengono creati nella direzione positiva della coordinata
	 // if deltaPos.coordinate < 0 i chunk vengono creati nella direzione negativa della coordinata

	 // this gives the right positions to the chunks but they are still not sorted
	 // m_Chunks needs to be then sorted by sorting the "subarrays" with the same z coordinate

	 // loop through all the chunks, the new chunk position is the new player grid position - the offset from the old player grid position and the chunk
	 // then delete the old chunk and have m_Chunks[i] Chunk pointer point to the new allocated chunk

	 for (int i = 0; i < m_Chunks.size(); i++)
	 {
		 Chunk* chunk = m_Chunks[i];
		 Mesh* mesh = chunk->GetMesh();

		 vec3 offsetFromLastGridPos = player->GetLastChunkGridPosition() - chunk->GetPosition();

		 vec3 distanceFromPlayer = player->GetChunkGridPosition() - chunk->GetPosition();

		 if (abs(distanceFromPlayer.x) > CHUNK_RADIUS || abs(distanceFromPlayer.z) > CHUNK_RADIUS)
		 {
			 vec3 newChunkPos = player->GetChunkGridPosition() + offsetFromLastGridPos;

			 Chunk* newChunk = new Chunk(newChunkPos, player, chunk->GetOffsetIntoBuffer());

			 delete chunk;
			 m_Chunks[i] = newChunk;
		 }
	 }

	 // sort the chunks by z coordinate first
	 insertion_sort_chunks(m_Chunks, 0, m_Chunks.size() - 1, Z_COORD);

	 // then sort subvector of the m_Chunks vector based on the x coordinate
	 int chunkRowLength = CHUNK_RADIUS * 2 + 1;

	 for (int i = 0; i < m_Chunks.size() / chunkRowLength; i++)
	 {
		 int start = chunkRowLength * i;
		 int end = start + chunkRowLength - 1;

		 insertion_sort_chunks(m_Chunks, start, end, X_COORD);
	 }

	 // start generating meshes from the first one when player moves between chunks

	 m_CurrentChunk = 0;
	 GenerateMeshes(m_Chunks[m_CurrentChunk], m_CurrentChunk);
 }

 void Terrain::GenerateMeshes(Chunk* chunk, int chunkNum)
 {
	 Mesh* mesh = chunk->GetMesh();
	 Player* player = chunk->GetPlayer();

	 SetChunkSurroundings(chunk, player->GetChunkGridPosition(), chunkNum);

	 mesh->Build(m_Buffers);
 }

 void Terrain::SetChunkSurroundings(Chunk* chunk, vec3 chunkPlayerIsIn, int indexInTerrain)
 {
	 // x and z coordinates of the chunk the player is in
	 // they don't change as the for loop iterates through the chunks in m_Chunks vector
	 int playerChunkX = chunkPlayerIsIn.x;
	 int playerChunkZ = chunkPlayerIsIn.z;

	 // calculate chunk index from 0 to m_Chunks.size() based on its position
	 vec3 chunkPos = chunk->GetPosition();

	 //allocated memory for 4 pointers to the surrounding chunks
	 Chunk** surr = (Chunk**)malloc(sizeof(Chunk*) * 4);

	 if (surr == NULL)
	 {
		 printf("surrounding chunks not allocated");
		 return;
	 }

	 // CHUNK_RADIUS * 2 + 1: length of a row of chunks

	 if (chunkPos.x - 1 < playerChunkX - CHUNK_RADIUS)
		 surr[left_chunk] = nullptr;
	 else
		 surr[left_chunk] = m_Chunks[indexInTerrain - 1];

	 if (chunkPos.x + 1 > playerChunkX + CHUNK_RADIUS)
		 surr[right_chunk] = nullptr;
	 else
		 surr[right_chunk] = m_Chunks[indexInTerrain + 1];

	 if (chunkPos.z - 1 < playerChunkZ - CHUNK_RADIUS)
		 surr[back_chunk] = nullptr;
	 else
		 surr[back_chunk] = m_Chunks[indexInTerrain - (CHUNK_RADIUS * 2 + 1)];

	 if (chunkPos.z + 1 > playerChunkZ + CHUNK_RADIUS)
		 surr[front_chunk] = nullptr;
	 else
		 surr[front_chunk] = m_Chunks[indexInTerrain + (CHUNK_RADIUS * 2 + 1)];

	 chunk->SetSurroundings(surr);
 }