#pragma once

#include "Player/Player.h"

/*
	* pass a chunk to the class
	* iterate through the blocks of the chunk
	* add to the mesh only the vertices that are visible
	* the vertices of the faces inside the chunk shouldn't be added
*/

typedef struct {
	Buffer* model;
	Buffer* tex;
	Buffer* face_index;
} terrain_buffers;

// value for the offsets of the mesh data into the opengl buffers
typedef struct {
	unsigned int model;
	unsigned int tex;
	unsigned int face_index;
} offsets;

class Chunk; //forward declaration
class Player;

class Mesh
{
public:
	Mesh(Chunk* chunk);
	~Mesh();
	Mesh& operator= (Mesh& other);

	std::vector<int> GetFacesIndex();

	void Build(terrain_buffers* terrainBufs);

	void AddBlockToMesh(Block* block);
	void AddFaceToMesh(Block* block, uint8_t side);
	bool IsAdjacentBlockSolid(Block* block, vec3 adjBlockPos, Chunk* adjChunk);

	terrain_buffers* GetTerrainBufs();
	//offsets* GetTerrainOffsets();
	offsets GetTerrainOffsets();

	std::vector<Block*> m_BlocksAddedToMesh;
	//unsigned int m_FacesAddedToMesh;

private:
	Chunk* m_Chunk;
	std::vector<float> m_TexCoords;
	std::vector<int> m_Faces; // vector of enum sides values (see mesh.h) then interpreted correctly with glVertexAttriPointer in renderer->Draw()
	std::vector<float> m_ModelMats;

	terrain_buffers* m_TerrainBuffers; // all meshes point to this struct, every mesh instance has a different offset in the buffer dataset
	//offsets* m_TerrainOffsets;
	offsets m_TerrainOffsets;
};