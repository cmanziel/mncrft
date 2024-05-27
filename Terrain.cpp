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

	 // if player has moved between chunks, rearrange them and restart building the meshes accordingly
	 if (chunkX != lastChunkGridPosition.x || chunkZ != lastChunkGridPosition.z)
	 {
		 for (int i = 0; i < m_Chunks.size(); i++)
		 {
			 Chunk* chunk = m_Chunks[i];
			 Mesh* mesh = chunk->GetMesh();

			 vec3 offsetFromLastGridPos = player->GetLastChunkGridPosition() - chunk->GetPosition();

			 vec3 distanceFromPlayer = player->GetChunkGridPosition() - chunk->GetPosition();

			 if (abs(distanceFromPlayer.x) > CHUNK_RADIUS || abs(distanceFromPlayer.z) > CHUNK_RADIUS)
			 {
				 vec3 newChunkPos = player->GetChunkGridPosition() + offsetFromLastGridPos;

				 Chunk* newChunk = new Chunk(newChunkPos, player, chunk->GetOffsetIntoBuffer()); // give to the new chunk the offset in the buffer of the chunk that's being replaced

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

		 m_CurrentChunk = 0;
	 }

	 if (m_CurrentChunk >= m_Chunks.size())
	 {
		 m_CurrentChunk = 0;
		 //player->SetLastChunkGridPosition();
	 }

	 // start generating meshes from the first one when player moves between chunks
	 GenerateMeshes(m_Chunks[m_CurrentChunk], m_CurrentChunk);
	 m_CurrentChunk++;
 }

 void Terrain::GenerateMeshes(Chunk* chunk, int chunkNum)
 {
	 // TODO: check here if chunk is behind camera or not
	 // if it isn't, call mesh->Build()
	 Mesh* mesh = chunk->GetMesh();
	 Player* player = chunk->GetPlayer();

	 if (!player->GetCam()->IsPosInFrontOfCamera(player->GetChunkGridPosition(), chunk->GetPosition()))
		 return;

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
	 Chunk* surr[4];

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