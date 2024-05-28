#include "Terrain.h"
#include "Chunk/Chunk.h"
#include "utils/sort.h"
#include <iostream>

 Terrain::Terrain(Player* player)
 {
	 int chunkX = player->GetChunkGridPosition().x;
	 int chunkZ = player->GetChunkGridPosition().z;

	 unsigned int chunk_offset = 0;

	 m_TotalChunks = (CHUNK_RADIUS * 2 + 1) << 1;

	 // fill the m_Positions array
	 unsigned int pos_index = 0;
	 for (int z = chunkZ - CHUNK_RADIUS; z <= chunkZ + CHUNK_RADIUS; z++)
	 {
		 for (int x = chunkX - CHUNK_RADIUS; x <= chunkX + CHUNK_RADIUS; x++)
		 {
			 m_Positions[pos_index] = vec3(x, 0, z);

			 pos_index++;
		 }
	 }

	 // iterate through m_Positions, pass them to camera->IsPosInFrontOfCamera(), allocate just the ones in front of the camera
	 for (int i = 0; i < m_TotalChunks; i++)
	 {
		 if (player->GetCam()->IsPosInFrontOfCamera(player->GetChunkGridPosition(), m_Positions[i]))
		 {
			 Chunk* chunk = new Chunk(m_Positions[i], player, chunk_offset);

			 m_Chunks.push_back(chunk);

			 chunk_offset++;
		 }
	 }

	 // allocate terrain buffers
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

	 // generate the meshes for the allocated chunks
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

 void Terrain::GenerateWorld(Player* player)
 {
	 int chunkX = player->GetChunkGridPosition().x;
	 int chunkZ = player->GetChunkGridPosition().z;

	 vec3 lastChunkGridPosition = player->GetLastChunkGridPosition();

	 // if player has moved between chunks, rearrange them and restart building the meshes accordingly
	 if (chunkX != lastChunkGridPosition.x || chunkZ != lastChunkGridPosition.z)
	 {
		 std::vector<vec3> positions_in_front;

		 // redefine the positions in the m_Positions array and save the ones in front of the camera
		 unsigned int pos_index = 0;
		 for (int z = chunkZ - CHUNK_RADIUS; z <= chunkZ + CHUNK_RADIUS; z++)
		 {
			 for (int x = chunkX - CHUNK_RADIUS; x <= chunkX + CHUNK_RADIUS; x++)
			 {
				 vec3 pos = vec3(x, 0, z);

				 m_Positions[pos_index] = vec3(x, 0, z);
				 pos_index++;

				 if (player->GetCam()->IsPosInFrontOfCamera(player->GetChunkGridPosition(), pos))
					 positions_in_front.push_back(pos);
			 }
		 }

		 // iterate through pos and overwrite what's in m_Chunks and if necessary push_back new chunks
		 unsigned int size = m_Chunks.size();
		 for (int i = 0; i < positions_in_front.size(); i++)
		 {
			 if (i < size)
			 {
				 delete m_Chunks[i];

				 m_Chunks[i] = new Chunk(positions_in_front[i], player, i);
			 }
			 else
			 {
				 Chunk* newChunk = new Chunk(positions_in_front[i], player, i);

				 m_Chunks.push_back(newChunk);
			 }
		 }

		 m_CurrentChunk = 0;
	 }

	 if (m_CurrentChunk >= m_Chunks.size())
		 m_CurrentChunk = 0;

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

	 // TODO: modify this function according to the new m_Chunks vector layout
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

 std::vector<Chunk*> Terrain::GetChunks()
 {
	 return m_Chunks;
 }