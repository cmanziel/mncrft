#include "Terrain.h"
#include "../Chunk/Chunk.h"
#include "../utils/sort.h"
#include <iostream>

Terrain::Terrain(Player* player)
	: m_Player(player), m_BlockPointed(NULL)
{
	m_CurrentChunk = 0;
	m_TotalChunks = (CHUNK_RADIUS * 2 + 1) * (CHUNK_RADIUS * 2 + 1);
	m_NoiseMap = new NoiseMap(CHUNK_RADIUS * 2 + 1);

	int chunkX = m_Player->GetChunkGridPosition().x;
	int chunkZ = m_Player->GetChunkGridPosition().z;

	int startZ = chunkZ - CHUNK_RADIUS;
	int startX = chunkX - CHUNK_RADIUS;
	unsigned int chunk_offset = 0; // offset in the buffer allocated in the gpu
	for (int z = 0; z < CHUNK_RADIUS * 2 + 1; z++)
	{
		for (int x = 0; x < CHUNK_RADIUS * 2 + 1; x++)
		{
			vec3 pos = vec3(startX + x, 0, startZ + z);

			//unsigned int solid_height = m_NoiseMap->GetGridValues()[z][x] * CHUNK_HEIGHT;
			unsigned int solid_height = CHUNK_HEIGHT * m_NoiseMap->GetValue(x, z, 1.0f);

			Chunk* chunk = new Chunk(pos, m_Player, chunk_offset, solid_height);

			m_Chunks[z][x] = chunk;

			chunk_offset++;
		}
	}

	// allocate buffers for model matrices, texture coordinates and faces indexe
	// model buffer: allocate 16 float values for every face of every block, that's a lot more than the matrices that will be calculated in the mesh
	unsigned int num_of_faces = m_Chunks[0][0]->GetBlocksVector().size() * FACES_PER_BLOCK;

	unsigned int modelBufSize = 16 * sizeof(float) * num_of_faces * m_TotalChunks;
	unsigned int texCoordsBufSize = 2 * sizeof(float) * INDICES_PER_FACE * num_of_faces * m_TotalChunks;
	unsigned int facesIndexBufSize = sizeof(int) * num_of_faces * m_TotalChunks;
	unsigned int pointedFlagsSize = sizeof(float) * num_of_faces * m_TotalChunks;

	m_Buffers = (terrain_buffers*)malloc(sizeof(terrain_buffers));

	if (m_Buffers == NULL)
	{
		printf("terrain buffers not allocated");
		return;
	}

	m_Buffers->model = new Buffer((float*)NULL, modelBufSize);
	m_Buffers->tex = new Buffer((float*)NULL, texCoordsBufSize);
	m_Buffers->face_index = new Buffer((float*)NULL, facesIndexBufSize);
	m_Buffers->pointedFlags = new Buffer((float*)NULL, pointedFlagsSize);

	BreakBlock();
	// create the meshes
	for (int z = 0; z < CHUNK_RADIUS * 2 + 1; z++)
	{
		for (int x = 0; x < CHUNK_RADIUS * 2 + 1; x++)
		{
			//Chunk* c = m_Chunks[z][x];
			//Mesh* mesh = c->GetMesh();

			//// using the chunk_offset for setting the chunk surroundings could be a problem if chunks are sorted
			//SetChunkSurroundings(c, vec3(x, 0, z));

			//mesh->Build(m_Buffers);
			GenerateMeshes(m_CurrentChunk);
			m_CurrentChunk++;
		}
	}
}

Terrain::~Terrain()
{
	delete m_Buffers->model;
	delete m_Buffers->tex;
	delete m_Buffers->face_index;
	delete m_Buffers->pointedFlags;

	delete m_NoiseMap;

	free(m_Buffers);

	// delete every chunk in the grid
	for (int z = 0; z < CHUNK_RADIUS * 2 + 1; z++)
	{
		for (int x = 0; x < CHUNK_RADIUS * 2 + 1; x++)
		{
			delete m_Chunks[z][x];
		}
	}
}

Player* Terrain::GetPlayer()
{
	return m_Player;
}

void Terrain::GenerateWorld()
{
	// do block breaking here before the meshes are generated
	BreakBlock();

	// Wrap chunk rearranging in a separate function
	int chunkX = m_Player->GetChunkGridPosition().x;
	int chunkZ = m_Player->GetChunkGridPosition().z;

	vec3 lastChunkGridPosition = m_Player->GetLastChunkGridPosition();

	// if player has moved between chunks, rearrange them and restart building the meshes accordingly
	if (chunkX != lastChunkGridPosition.x || chunkZ != lastChunkGridPosition.z)
	{
		RearrangeChunks();
		// start generating meshes from the first one when player moves between chunks
		m_CurrentChunk = 0;
	}

	if (m_CurrentChunk > m_TotalChunks - 1)
		m_CurrentChunk = 0;

	GenerateMeshes(m_CurrentChunk);
	m_CurrentChunk++;
}

void Terrain::GenerateMeshes(unsigned int currentChunk)
{
	// z and x index from m_CurrentChunk: z = m_CurrentChunk / (CHUNK_RADIUS * 2 + 1); x = m_CurrentChunk % (CHUNK_RADIUS * 2 + 1)
	int currentZ = m_CurrentChunk / (CHUNK_RADIUS * 2 + 1);
	int currentX = m_CurrentChunk % (CHUNK_RADIUS * 2 + 1);

	vec3 gridIndex = vec3(currentX, 0, currentZ);

	Chunk* chunk = m_Chunks[currentZ][currentX];

	Mesh* mesh = chunk->GetMesh();

	if (!m_Player->GetCam()->IsPosInFrontOfCamera(m_Player->GetChunkGridPosition(), chunk->GetPosition()))
		return;

	SetChunkSurroundings(chunk, gridIndex);

	chunk->BuildMesh(m_Buffers, m_BlockPointed);
}

void Terrain::SetChunkSurroundings(Chunk* chunk, vec3 gridIndex)
{
	int chunkX = gridIndex.x;
	int chunkZ = gridIndex.z;

	Chunk* surr[4];

	if (gridIndex.x - 1 < 0)
		surr[left_chunk] = NULL;
	else
		surr[left_chunk] = m_Chunks[chunkZ][chunkX - 1];

	if (gridIndex.x + 1 >= CHUNK_RADIUS * 2 + 1)
		surr[right_chunk] = NULL;
	else
		surr[right_chunk] = m_Chunks[chunkZ][chunkX + 1];

	if (gridIndex.z - 1 < 0)
		surr[back_chunk] = NULL;
	else
		surr[back_chunk] = m_Chunks[chunkZ - 1][chunkX];

	if (gridIndex.z + 1 >= CHUNK_RADIUS * 2 + 1)
		surr[front_chunk] = NULL;
	else
		surr[front_chunk] = m_Chunks[chunkZ + 1][chunkX];

	chunk->SetSurroundings(surr);
}

void Terrain::UpdatePlayerChunkGridPosition()
{
	m_Player->SetLastChunkGridPosition();

	vec3 playerPos = m_Player->GetCam()->GetPosition();

	// z: floor(playerPos.z / CHUNK_SIZE)

	vec3 playerChunkGridPos = vec3(floor(playerPos.x / CHUNK_SIZE), 0, floor(playerPos.z / CHUNK_SIZE));

	m_Player->UpdateChunkGridPosition(playerChunkGridPos);
}

void Terrain::RearrangeChunks()
{
	int chunkX = m_Player->GetChunkGridPosition().x;
	int chunkZ = m_Player->GetChunkGridPosition().z;

	vec3 lastChunkGridPosition = m_Player->GetLastChunkGridPosition();

	vec3 deltaPos = m_Player->GetChunkGridPosition() - lastChunkGridPosition;

	// "center" of the bidimensional array of chunk which x and z go from 0 to CHUNK_RADIUS * 2 + 1
	vec3 chunkGridCenter = vec3(CHUNK_RADIUS, 0, CHUNK_RADIUS);

	vec3 offset = vec3(deltaPos.x * CHUNK_RADIUS, 0, deltaPos.z * CHUNK_RADIUS);
	//vec3 offset = CHUNK_RADIUS * deltaPos;

	// chunkGridCenter - offset = center of the row or column to delete, if deltaPos.x and deltaPos.z != 0 it is the intersectiomn between the column and row to delete

	// delete column and allocate new Chunks with new positions
	if (offset.x != 0)
	{
		int columnX = chunkGridCenter.x - offset.x; // this is either 0 or CHUNK_RADIUS * 2

		std::vector<Chunk*> newColumn(CHUNK_RADIUS * 2 + 1);

		for (int z = 0; z < CHUNK_RADIUS * 2 + 1; z++)
		{
			// allocate new Chunk with correct position, and assign to it the to-be-replaced chunk's offset into the terrain buffers
			Chunk* toReplace = m_Chunks[z][columnX];
			vec3 newPos = vec3(chunkX + offset.x, 0, chunkZ - CHUNK_RADIUS + z);
			Chunk* newChunk = new Chunk(newPos, m_Player, toReplace->GetOffsetIntoBuffer(), toReplace->m_LowestSolidHeight);

			newColumn[z] = newChunk;

			delete toReplace;

			m_Chunks[z][columnX] = newChunk;
		}

		// translate every column by one in the direction opposite to the movement and place the new one in the freed space
		// x += deltaPos.x, x increases or decreases depending on the direction of the movement
		int i = 0;
		int x = columnX;
		for (; i < CHUNK_RADIUS * 2; i++)
		{
			for (int z = 0; z < CHUNK_RADIUS * 2 + 1; z++)
			{
				int ind = x + deltaPos.x;
				m_Chunks[z][x] = m_Chunks[z][ind];
			}

			x += deltaPos.x;
		}

		// subsitute the last shifted column with the new one
		for (int z = 0; z < CHUNK_RADIUS * 2 + 1; z++)
		{
			m_Chunks[z][x] = newColumn[z];
		}
	}

	// delete row and new Chunks along z axis, actually if deltaPos results != 0 both on x and y, the chunk at the intersection of the column and row will be processed twice (deleted and then a new is allocated on the column then the same for the row)
	if (offset.z != 0)
	{
		int rowZ = chunkGridCenter.z - offset.z;

		std::vector<Chunk*> newRow(CHUNK_RADIUS * 2 + 1);
		for (int x = 0; x < CHUNK_RADIUS * 2 + 1; x++)
		{
			Chunk* toReplace = m_Chunks[rowZ][x];
			vec3 newPos = vec3(chunkX - CHUNK_RADIUS + x, 0, chunkZ + offset.z);
			Chunk* newChunk = new Chunk(newPos, m_Player, toReplace->GetOffsetIntoBuffer(), toReplace->m_LowestSolidHeight);

			newRow[x] = newChunk;

			delete toReplace;

			m_Chunks[rowZ][x] = newChunk;
		}

		int i = 0;
		int z = rowZ;
		for (; i < CHUNK_RADIUS * 2; i++)
		{
			int ind = z + deltaPos.z;

			for (int x = 0; x < CHUNK_RADIUS * 2 + 1; x++)
			{
				m_Chunks[z][x] = m_Chunks[ind][x];
			}

			z += deltaPos.z;
		}

		for (int x = 0; x < CHUNK_RADIUS * 2 + 1; x++)
		{
			m_Chunks[z][x] = newRow[x];
		}
	}
}

void Terrain::BreakBlock()
{
	// get the player's position in the chunk grid

	int playerGridX = m_Player->GetChunkGridPosition().x;
	int playerGridZ = m_Player->GetChunkGridPosition().z;

	// increment the value of t for the player's ray
	// determine in which chunk the relative point is in case the ray intersects a block in another chunk
	// determine the block's local position by flooring the point's coordinates
	// get the block from the chunk's block vector
	// if the block's solid, set it's id to air

	Ray ray = Ray(m_Player->GetCam()->GetCameraFront(), m_Player->GetCam()->GetDirection());

	for (float t = 0.0; t < m_Player->m_BreakMaxDistance; t += 0.2)
	{
		point p = ray.at(t);

		int pointXFromPlayer = floor(p.x / CHUNK_SIZE) - playerGridX; // if = 0 the point is still in the same chunk
		int pointZFromPlayer = floor(p.z / CHUNK_SIZE) - playerGridZ;

		// the indexes in m_Chunks for the point are:
		// player's indexes in m_CHunks: m_Chunks[CHUNK_RADIUS][CHUNK_RADIUS]
		// point: m_Chunks[CHUNK_RADIUS + pointZFromPlayer][CHUNK_RADIUS + pointXfFromPlayer]

		int pointX = CHUNK_RADIUS + pointXFromPlayer;
		int pointZ = CHUNK_RADIUS + pointZFromPlayer;

		Chunk* chunk = m_Chunks[pointZ][pointX];

		// evaluate the block's local position

		int pLocalX = floor(p.x) - floor(p.x / CHUNK_SIZE) * CHUNK_SIZE;
		int pLocalZ = floor(p.z) - floor(p.z / CHUNK_SIZE) * CHUNK_SIZE;

		vec3 pointLocalPosition = vec3(pLocalX, floor(p.y), pLocalZ);

		Block* blockToBreak = chunk->GetBlock(pointLocalPosition);

		if (blockToBreak == NULL)
			return;

		m_BlockPointed = blockToBreak;

		if (blockToBreak->GetID() != air)
		{
			//blockToBreak->m_IsPointed = true;

			if (m_Player->GetState() != STATE_BREAK)
				return;

			blockToBreak->SetID(air);

			if (blockToBreak->GetLocalPosition().y == chunk->m_LowestSolidHeight && chunk->m_LowestSolidHeight - 1 > 0)
				chunk->m_LowestSolidHeight--;

			// set the surrounding chunk m_LoestSolidHeight field to the lowest out of the four
			uint8_t lowest = chunk->m_LowestSolidHeight;

			for (int i = 0; i < 4; i++)
			{
				if (chunk->GetSurrounding()[i]->m_LowestSolidHeight < lowest)
					lowest = chunk->GetSurrounding()[i]->m_LowestSolidHeight;
			}

			chunk->m_LowestSolidHeight = lowest;

			for (int i = 0; i < 4; i++)
			{
				chunk->GetSurrounding()[i]->m_LowestSolidHeight = lowest;
			}

			return; // as soon as a solid block is encountered exit the function
		}
	}
}