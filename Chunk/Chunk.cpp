#include "Chunk.h"

//int column[CHUNK_SIZE][CHUNK_SIZE];

//Chunk::Chunk(vec3 position)
//	: m_Position(position)
//{
//	m_NoiseMap = new NoiseMap(CHUNK_SIZE);
//	m_Mesh = new Mesh(this);
//
//	for (int x = 0; x < CHUNK_SIZE; x++)
//	{
//		for (int z = 0; z < CHUNK_SIZE; z++)
//		{
//			int air_blocks = (int)(3 * m_NoiseMap->GetGridValues()[x][z]);
//			int col_height = CHUNK_HEIGHT - air_blocks;
//
//			for (int y = 0; y < CHUNK_HEIGHT; y++)
//			{
//				unsigned int ID;
//
//				if (y == CHUNK_HEIGHT - 1)
//					ID = grass;
//				else if (y < 13)
//					ID = cobblestone;
//				else
//					ID = dirt;
//
//				vec3 blockWorldPos = vec3(m_Position.x * CHUNK_SIZE + x, y, m_Position.z * CHUNK_SIZE + z);
//
//				Block* block = new Block(vec3(x, y, z), blockWorldPos, ID);
//				m_Blocks.push_back(block);
//			}
//			column[x][z] = col_height - 1;
//		}
//	}
//}

//chunk creation: 16x16x16 blocks
//Chunk::Chunk(vec3 position) {
//	m_Position = position;
//
//	int random_height = rand() % 16;
//
//	for (int y = 0; y < random_height; y++)
//	{
//		for (int z = 0; z < CHUNK_SIZE; z++)
//		{
//			for (int x = 0; x < CHUNK_SIZE; x++)
//			{
//				unsigned int ID;
// 
//				if (y == CHUNK_HEIGHT - 1)
//					ID = grass;
//				else if (y < 13)
//					ID = cobblestone;
//				else
//					ID = dirt;
//
//				Block* block = new Block(vec3(x, y, z), m_Position, ID); // give the block a position based on the chunk position and its position in the chunk
//				m_Blocks.push_back(block);
//			}
//		}
//	}
//}

Chunk::Chunk(vec3 position, Player* player, unsigned int offset)
	: m_Position(position), m_Player(player), m_OffsetIntoBuffer(offset)
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

	m_Mesh = new Mesh(this);
}

//Chunk::Chunk(vec3 position, Player* player, unsigned int offset)
//	: m_Position(position), m_Player(player), m_OffsetIntoBuffer(offset)
//{
//	m_LowestSolidHeight = 0;
//
//	m_NoiseMap = new NoiseMap(CHUNK_SIZE);
//
//	for (int x = 0; x < CHUNK_SIZE; x++)
//	{
//		for (int z = 0; z < CHUNK_SIZE; z++)
//		{
//			int air_blocks = (int)(CHUNK_HEIGHT / 3 * m_NoiseMap->GetGridValues()[x][z]);
//			int col_height = CHUNK_HEIGHT - air_blocks; // height of solid blocks' column
//
//			if (x == 0 && z == 0)
//				m_LowestSolidHeight = col_height;
//			else if (col_height < m_LowestSolidHeight)
//				m_LowestSolidHeight = col_height;
//
//			for (int y = 0; y < CHUNK_HEIGHT; y++)
//			{
//				short ID;
//
//				if (y >= col_height)
//					ID = air;
//				else if (y == col_height - 1)
//					ID = grass;
//				else if (y < CHUNK_HEIGHT / 2)
//					ID = cobblestone;
//				else
//					ID = dirt;
//
//				vec3 blockWorldPos = vec3(m_Position.x * CHUNK_SIZE + x, y, m_Position.z * CHUNK_SIZE + z);
//
//				Block* block = new Block(vec3(x, y, z), blockWorldPos, ID); // give the block a position based on the chunk position and its position in the chunk
//				m_Blocks.push_back(block);
//			}
//		}
//	}
//
//	m_Mesh = new Mesh(this);
//}

// copy constructor
Chunk::Chunk(Chunk& other, Player* player)
{
	m_Position = other.m_Position;

	m_NoiseMap = new NoiseMap(*other.m_NoiseMap);

	m_OffsetIntoBuffer = other.m_OffsetIntoBuffer;

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

Block* Chunk::GetBlock(vec3 blockPos)
{
	if (blockPos.x > CHUNK_SIZE - 1 || blockPos.z > CHUNK_SIZE - 1)
	{
		return nullptr;
	}

	if (blockPos.y > CHUNK_HEIGHT - 1)
		return nullptr;

	if (blockPos.x < 0 || blockPos.y < 0 || blockPos.z < 0)
		return nullptr;

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

// TODO: the blocks' world positions should be updated when a chunk position is modified via Terrain::GenerateWorld
void Chunk::SetPosition(vec3 pos)
{
	m_Position = pos;
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

//Block* Chunk::GetBlock(vec3 position) {
//	if (position.x > CHUNK_SIZE - 1 || position.y > CHUNK_HEIGHT - 1 || position.z > CHUNK_SIZE -  1)
//		return nullptr;
//
//	if (position.x < 0 || position.y < 0 || position.z < 0)
//		return nullptr;
//
//	int index = position.y * CHUNK_SIZE * CHUNK_SIZE + position.z * CHUNK_SIZE + position.x;
//
//	return m_Blocks[index];
//}

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

// this function is used for assigning one chunk to another: chunk1 = chunk2
// it is useful because it does not leave the job of copying the class to the compiler
// and also in this case it's better not to just assign the pointer of a chunk to another because it can cause problems like deleting two chunks that in reality point to the same one memory address
// so the second deletion will crash the program, like it happens when sorting the chunks 

//Chunk Chunk::operator= (Chunk other)
//{
//	printf("tatatata\n");
//
//	if (this == &other)
//		return *this;
//
//	this->m_Position = other.m_Position;
//
//	this->m_NoiseMap = other.m_NoiseMap;
//
//	this->m_Surrounding[left] = other.m_Surrounding[left];
//	this->m_Surrounding[right] = other.m_Surrounding[right];
//	this->m_Surrounding[back] = other.m_Surrounding[back];
//	this->m_Surrounding[front] = other.m_Surrounding[front];
//
//	for (int i = 0; i < this->m_Blocks.size(); i++)
//	{
//		this->m_Blocks[i] = other.m_Blocks[i];
//	}
//
//	this->m_Mesh = other.m_Mesh;
//
//	return *this;
//}

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

	m_Player = other.m_Player;

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