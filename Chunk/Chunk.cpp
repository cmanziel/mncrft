#include "Chunk.h"

Chunk::Chunk(vec3 position, Player* player, unsigned int offset)
	: m_Position(position), m_OffsetIntoBuffer(offset), m_Player(player)
{
	m_LowestSolidHeight = 0;
	m_NoiseMap = new NoiseMap(CHUNK_SIZE);

	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				int air_blocks = (int)(CHUNK_HEIGHT / 3 * m_NoiseMap->GetGridValues()[z][x]);
				int col_height = CHUNK_HEIGHT - air_blocks;

				if (x == 0 && z == 0)
					m_LowestSolidHeight = col_height;
				else if (col_height < m_LowestSolidHeight)
					m_LowestSolidHeight = col_height;

				short ID;

				if (y >= col_height)
					ID = air;
				else if (y == col_height - 1)
					ID = grass;
				else if (y < CHUNK_HEIGHT / 2)
					ID = cobblestone;
				else
					ID = dirt;

				vec3 blockWorldPos = vec3(m_Position.x * CHUNK_SIZE + x, y, m_Position.z * CHUNK_SIZE + z);

				Block* block = new Block(vec3(x, y, z), blockWorldPos, ID); // give the block a position based on the chunk position and its position in the chunk

				m_Blocks.push_back(block);
			}
		}
	}

	m_Mesh = new Mesh();
}

// copy constructor
Chunk::Chunk(Chunk& other)
{
	m_Position = other.m_Position;

	m_NoiseMap = new NoiseMap(*other.m_NoiseMap);

	m_OffsetIntoBuffer = other.m_OffsetIntoBuffer;

	m_LowestSolidHeight = other.m_LowestSolidHeight;

	m_Player = other.m_Player;

	// set surroudning chunks:
	// iterate through other's m_Surrounding array and copy values

	for (int i = 0; i < 4; i++)
	{
		m_Surrounding[i] = other.m_Surrounding[i];
	}

	// operator= defined in Mesh class
	// by doing m_Mesh = other.m_Mesh you are still assignign pointers to one another
	// so the debugger won't step into the operator= function because the operands of the assignment don't correspond

	// problem, see Mesh.cpp, this causes infinite loop
	//*m_Mesh = *(other.m_Mesh);

	// iterate through other's block's vector and copy them
	for (int i = 0; i < other.m_Blocks.size(); i++)
	{
		Block* b = new Block(*other.m_Blocks[i]);
		m_Blocks.push_back(b);
	}

	//m_Mesh = new Mesh(&other);
}

// block local position is passed to the function
//Block* Chunk::GetBlock(vec3 blockPos)
//{
//	if (blockPos.x > CHUNK_SIZE - 1 || blockPos.z > CHUNK_SIZE - 1)
//	{
//		return nullptr;
//	}
//
//	// local position of block in chunk
//	int localX = blockPos.x;
//	int localZ = blockPos.z;
//
//	if (blockPos.y > CHUNK_HEIGHT - 1)
//	{
//		return nullptr;
//	}
//
//	if (blockPos.x < 0 || blockPos.y < 0 || blockPos.z < 0)
//		return nullptr;
//
//	int index = blockPos.x * CHUNK_SIZE * CHUNK_HEIGHT + blockPos.z * CHUNK_HEIGHT + blockPos.y;
//
//	return m_Blocks[index];
//}

// index in the m_Blocks vector from x 
Block* Chunk::GetBlock(vec3 blockPos)
{
	if (blockPos.x > CHUNK_SIZE - 1 || blockPos.z > CHUNK_SIZE - 1)
	{
		return NULL;
	}

	if (blockPos.y > CHUNK_HEIGHT - 1)
		return NULL;

	if (blockPos.x < 0 || blockPos.y < 0 || blockPos.z < 0)
		return NULL;

	unsigned int index = blockPos.y * CHUNK_SIZE * CHUNK_SIZE + blockPos.z * CHUNK_SIZE + blockPos.x;

	return m_Blocks[index];
}

void Chunk::SetSurroundings(Chunk** surr)
{
	for (int i = 0; i < 4; i++)
	{
		m_Surrounding[i] = surr[i];
	}
}

Chunk** Chunk::GetSurrounding() {
	return m_Surrounding;
}

NoiseMap* Chunk::GetNoiseMap()
{
	return m_NoiseMap;
}

Player* Chunk::GetPlayer()
{
	return m_Player;
}

vec3 Chunk::GetPosition()
{
	return m_Position;
}

Mesh* Chunk::GetMesh()
{
	return m_Mesh;
}

std::vector<Block*> Chunk::GetBlocksVector()
{
	return m_Blocks;
}

unsigned int Chunk::GetOffsetIntoBuffer()
{
	return m_OffsetIntoBuffer;
}

unsigned int Chunk::GetLowestSolidHeight()
{
	return m_LowestSolidHeight;
}

Chunk::~Chunk() {
	delete m_Mesh;
	delete m_NoiseMap;
	//delete m_Surrounding;

	// free the memory of m_Surrounding field
	// 4 because there are 4 directions for surrounding chunks: left, right, back, front
	//for (int i = 0; i < 4; i++)
	//{
	//	free(m_Surrounding[i]);
	//}

	//free(m_Surrounding);

	for (Block* b : m_Blocks)
		delete b;

	m_Blocks.clear();
}

Chunk& Chunk::operator= (Chunk& other)
{
	if (this == &other)
		return *this;

	m_Position = other.m_Position;

	m_OffsetIntoBuffer = other.m_OffsetIntoBuffer;

	*m_NoiseMap = *other.m_NoiseMap;

	// for m_Surrounding assign just the pointers
	/*m_Surrounding = other.m_Surrounding;*/

	for (int i = 0; i < 4; i++)
	{
		m_Surrounding[i] = other.m_Surrounding[i];
	}

	//// here assign pointers because memory for every surrounding chunk was not allocated
	//for (int i = 0; i < 4; i++)
	//{
	//	// TODO: problem: if other.m_Surrounding is NULL
	//	// then the chunk operator= tries to access and assign members of a NULL instance of Chunk class
	//	m_Surrounding[i] = other.m_Surrounding[i];
	//}

	//*m_Mesh = *other.m_Mesh;

	// assign blocks
	for (int i = 0; i < other.m_Blocks.size(); i++)
	{
		*m_Blocks[i] = *other.m_Blocks[i];
	}

	return *this;
}

void Chunk::BuildMesh(terrain_buffers* terrainBufs)
{
	unsigned int num_of_faces = m_Blocks.size() * FACES_PER_BLOCK;

	offsets terrainOffsets;

	terrainOffsets.model = m_OffsetIntoBuffer * num_of_faces * 16 * sizeof(float);
	terrainOffsets.tex = m_OffsetIntoBuffer * num_of_faces * 2 * sizeof(float) * INDICES_PER_FACE;
	terrainOffsets.face_index = m_OffsetIntoBuffer * num_of_faces * sizeof(int);

	m_Mesh->Clear();

	// skip the whole section of the chunk before the first air block
	size_t start = (m_LowestSolidHeight - 1) * CHUNK_SIZE * CHUNK_SIZE;
	//size_t start = 0;

	for (size_t i = start; i < m_Blocks.size(); i++)
	{
		Block* block = m_Blocks[i];

		Camera* cam = m_Player->GetCam();
		bool isBlockInsideFrustum = cam->IsInsideFrustum(block->GetWorldPosition());
		//bool isBlockInsideFrustum = true;

		if (block->GetID() != air && isBlockInsideFrustum)
			AddBlockToMesh(block);
	}

	// after the blocks are evaluated call mesh->Build which sends the data to the buffer object
	m_Mesh->Build(terrainBufs, terrainOffsets);
}

void Chunk::AddBlockToMesh(Block* block)
{
	bool isAddedToMesh = false;

	vec3 blp = block->GetLocalPosition();
	Camera* cam = m_Player->GetCam();

	// adjacent blocks' positions relative to the current one's local position
	vec3 adjacentPositions[FACES_PER_BLOCK] = {
		vec3(blp.x, blp.y + 1, blp.z),
	    vec3(blp.x, blp.y - 1, blp.z),
		vec3(blp.x - 1, blp.y, blp.z),
		vec3(blp.x + 1, blp.y, blp.z),
		vec3(blp.x, blp.y, blp.z + 1),
		vec3(blp.x, blp.y, blp.z - 1)
	};

	for (int i = 0; i < FACES_PER_BLOCK; i++)
	{
		Chunk* adjChunk;
		if (i < 2)
			adjChunk = NULL;
		else
			adjChunk = m_Surrounding[i - 2];

		if (!IsAdjacentBlockSolid(block, adjacentPositions[i], adjChunk))
		{
			m_Mesh->AddFace(block, m_Player->GetCam(), i);
		}
	}
}

bool Chunk::IsAdjacentBlockSolid(Block* block, vec3 adjBlockPos, Chunk* adjChunk)
{
	Block* adjBlock = GetBlock(adjBlockPos);

	// if adjBlock == nullptr means that the block that's being checked to add to the mesh is a block on the edge of the chunk which mesh is being created
	if (adjBlock == NULL)
	{
		// if the chunk which mesh that's being created has no adjacent chunk, then the edge faces are all rendered
		if (adjChunk == NULL)
			return false;

		// intialize the adjacent block in the adjacent chunk's local position to the position of this block, then change its x and z coords properly
		vec3 blockPosInOtherChunk = block->GetLocalPosition();

		if (adjBlockPos.x > CHUNK_SIZE - 1)
			blockPosInOtherChunk.x = 0.0;

		if (adjBlockPos.x < 0)
			blockPosInOtherChunk.x = CHUNK_SIZE - 1;

		if (adjBlockPos.z > CHUNK_SIZE - 1)
			blockPosInOtherChunk.z = 0.0;

		if (adjBlockPos.z < 0)
			blockPosInOtherChunk.z = CHUNK_SIZE - 1;

		// if the position calculated has an air block placed at it, don't add face to the mesh
		// GetBlock function below won't return nullptr because the condition if the adjChunk is nullptr has alredy been checked
		if (adjChunk->GetBlock(blockPosInOtherChunk)->GetID() != air)
			return true;
	}
	else if (adjBlock->GetID() != air)
		return true;

	return false;
}