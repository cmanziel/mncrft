#pragma once

//#include "Chunk/Chunk.h"

#include "../Block/Block.h"
#include "../Camera/Camera.h"

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
	Buffer* pointedFlags;
} terrain_buffers;

// value for the offsets of the mesh data into the opengl buffers
typedef struct {
	unsigned int model;
	unsigned int tex;
	unsigned int face_index;
	unsigned int pointedFlags;
} offsets;

class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh& operator= (Mesh& other);

	std::vector<int> GetFacesIndex();

	void Clear();
	void Build(terrain_buffers* terrainBufs, offsets terrainBuffersOffsets);
	void AddFace(Block* block, Camera* cam, uint8_t side);

	void AddBlockToMesh(Block* block);

	terrain_buffers* GetTerrainBufs();

	//offsets* GetTerrainOffsets();
	offsets GetTerrainOffsets();

private:
	std::vector<float> m_TexCoords;
	std::vector<int> m_Faces; // vector of enum sides values (see mesh.h) then interpreted correctly with glVertexAttriPointer in renderer->Draw()
	std::vector<float> m_ModelMats;
	std::vector<float> m_PointedFlags; // vector for the values of m_IsPointed flag of the blocks added to the mesh

	terrain_buffers* m_TerrainBuffers; // all meshes point to this struct, every mesh instance has a different offset in the buffer dataset
	//offsets* m_TerrainOffsets;
	offsets m_TerrainOffsets;
};