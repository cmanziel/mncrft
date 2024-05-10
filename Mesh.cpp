#include "Mesh.h"
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <iostream>

// blockAddedToMesh field stores the blocks that have a face which vertices are added to the mesh
// in the renderer iterate through the blockAddedToMesh field set the MVP for the current vertices

void insertFaceTexCoords(float* coords, unsigned int blockID, int side);
//mat4 transform_face(Camera* camera, float* face, int side, vec3 blockWorldPos);

//enum sides {
//	front, back, left, right, top, bottom
//};

enum axis {
	x, y, z
};

//float face[] =
//{
//	0.0, 1.0, 0.0,
//	0.0, 0.0, 0.0,
//	1.0, 0.0, 0.0,
//	0.0, 1.0, 0.0,
//	1.0, 0.0, 0.0,
//	1.0, 1.0, 0.0
//};

//float face[] =
//{
//	// back
//	0.0, 1.0, 0.0,
//	0.0, 0.0, 0.0,
//	1.0, 0.0, 0.0,
//	0.0, 1.0, 0.0,
//	1.0, 0.0, 0.0,
//	1.0, 1.0, 0.0,
//
//	// front
//	0.0, 1.0, 1.0,
//	0.0, 0.0, 1.0,
//	1.0, 0.0, 1.0,
//	0.0, 1.0, 1.0,
//	1.0, 0.0, 1.0,
//	1.0, 1.0, 1.0,
//
//	//left
//	0.0, 1.0, 0.0,
//	0.0, 0.0, 0.0,
//	0.0, 0.0, 1.0,
//	0.0, 1.0, 0.0,
//	0.0, 0.0, 1.0,
//	0.0, 1.0, 1.0,
//
//	//right
//	1.0, 1.0, 1.0,
//	1.0, 0.0, 1.0,
//	1.0, 0.0, 0.0,
//	1.0, 1.0, 1.0,
//	1.0, 0.0, 0.0,
//	1.0, 1.0, 0.0,
//
//	//top
//	0.0, 1.0, 0.0,
//	0.0, 1.0, 1.0,
//	1.0, 1.0, 1.0,
//	0.0, 1.0, 0.0,
//	1.0, 1.0, 1.0,
//	1.0, 1.0, 0.0,
//
//	// bottom
//	1.0, 0.0, 1.0,
//	1.0, 0.0, 0.0,
//	0.0, 0.0, 0.0,
//	1.0, 0.0, 1.0,
//	0.0, 0.0, 0.0,
//	0.0, 0.0, 1.0
//};

Mesh::Mesh(Chunk* chunk)
	: m_Chunk(chunk)
{
	m_TerrainBuffers = NULL;
}

Mesh::~Mesh()
{
	//free(m_TerrainOffsets);
}

std::vector<int> Mesh::GetFacesIndex()
{
	return m_Faces;
}

terrain_buffers* Mesh::GetTerrainBufs()
{
	return m_TerrainBuffers;
}

//offsets* Mesh::GetTerrainOffsets()
//{
//	return m_TerrainOffsets;
//}

offsets Mesh::GetTerrainOffsets()
{
	return m_TerrainOffsets;
}

void Mesh::Build(terrain_buffers* terrainBufs)
{
	if(m_TerrainBuffers == NULL)
		m_TerrainBuffers = terrainBufs;

	unsigned int num_of_faces = m_Chunk->GetBlocksVector().size() * FACES_PER_BLOCK;

	m_TerrainOffsets.model = m_Chunk->GetOffsetIntoBuffer() * num_of_faces * 16 * sizeof(float);
	m_TerrainOffsets.tex = m_Chunk->GetOffsetIntoBuffer() * num_of_faces * 2 * sizeof(float) * INDICES_PER_FACE;
	m_TerrainOffsets.face_index = m_Chunk->GetOffsetIntoBuffer() * num_of_faces * sizeof(int);

	m_ModelMats.clear();
	m_TexCoords.clear();
	m_Faces.clear();

	//for (Block* block : m_Chunk->GetBlocksVector())
	//{
	//	if(block->GetID() != air)
	//		AddBlockToMesh(block);
	//}

	// skip the whole section of the chunk before the first air block
	size_t start = (m_Chunk->GetLowestSolidHeight() - 1) * CHUNK_SIZE * CHUNK_SIZE;

	for (size_t i = start; i < m_Chunk->GetBlocksVector().size(); i++)
	{
		Block* block = m_Chunk->GetBlocksVector()[i];

		Camera* cam = m_Chunk->GetPlayer()->GetCam();
		bool isBlockInsideFrustum = cam->IsInsideFrustum(block->GetWorldPosition());
		//bool isBlockInsideFrustum = true;

		if (block->GetID() != air && isBlockInsideFrustum)
			AddBlockToMesh(block);
	}

	m_TerrainBuffers->model->SubData(m_ModelMats.data(), m_ModelMats.size() * sizeof(float), m_TerrainOffsets.model);
	m_TerrainBuffers->tex->SubData(m_TexCoords.data(), m_TexCoords.size() * sizeof(float), m_TerrainOffsets.tex);
	m_TerrainBuffers->face_index->SubData_int(m_Faces.data(), m_Faces.size() * sizeof(int), m_TerrainOffsets.face_index);
}

// add vertices to the mesh by checking if adjacent blocks are solid or not
void Mesh::AddBlockToMesh(Block* block)
{
	bool isAddedToMesh = false;

	vec3 blp = block->GetLocalPosition();

	Chunk** surr = m_Chunk->GetSurrounding();

	// add face vertices to the mesh vertices
	// add texture coordinates accroding to block id here in a separate function and not for every block
	if (!IsAdjacentBlockSolid(block, vec3(blp.x, blp.y + 1, blp.z), nullptr)) {
		AddFaceToMesh(block, top);
		isAddedToMesh = true;
		m_Faces.push_back(top);
		//m_FacesAddedToMesh++;
	}

	if (!IsAdjacentBlockSolid(block, vec3(blp.x, blp.y - 1, blp.z), nullptr)) {
		AddFaceToMesh(block, bottom);
		isAddedToMesh = true;
		m_Faces.push_back(bottom);
		//m_FacesAddedToMesh++;
	}

	if (!IsAdjacentBlockSolid(block, vec3(blp.x - 1, blp.y, blp.z), surr[left_chunk])) {
		AddFaceToMesh(block, left);
		isAddedToMesh = true;
		m_Faces.push_back(left);
		//m_FacesAddedToMesh++;
	}

	if (!IsAdjacentBlockSolid(block, vec3(blp.x + 1, blp.y, blp.z), surr[right_chunk])) {
		AddFaceToMesh(block, right);
		isAddedToMesh = true;
		m_Faces.push_back(right);
		//m_FacesAddedToMesh++;
	}

	if (!IsAdjacentBlockSolid(block, vec3(blp.x, blp.y, blp.z + 1), surr[front_chunk])) {
		AddFaceToMesh(block, front);
		isAddedToMesh = true;
		m_Faces.push_back(front);
		//m_FacesAddedToMesh++;
	}

	if (!IsAdjacentBlockSolid(block, vec3(blp.x, blp.y, blp.z - 1), surr[back_chunk])) {
		AddFaceToMesh(block, back);
		isAddedToMesh = true;
		m_Faces.push_back(back);
		//m_FacesAddedToMesh++;
	}

	// if at least one face is added to the vertices array, add the block to the m_BlocksAddedToMesh vector
	if (isAddedToMesh) {
		m_BlocksAddedToMesh.push_back(block);
	}
}

// side: 6 different directions of the block faces
void Mesh::AddFaceToMesh(Block* block, uint8_t side)
{
	mat4 model = m_Chunk->GetPlayer()->GetCam()->GetModelMat4(block->GetWorldPosition(), side);

	// turn those matrices into float arrays, then insert them in the vectors
	float model_arr[16];

	mat4_to_float(model, model_arr);
	
	float texCoords[VALUES_PER_TEX_COORD * INDICES_PER_FACE];

	insertFaceTexCoords(texCoords, block->GetID(), side);

	m_ModelMats.insert(m_ModelMats.end(), model_arr, model_arr + 16);
	m_TexCoords.insert(m_TexCoords.end(), texCoords, texCoords + std::size(texCoords));
}

bool Mesh::IsAdjacentBlockSolid(Block* block, vec3 adjBlockPos, Chunk* adjChunk)
{
	Block* adjBlock = m_Chunk->GetBlock(adjBlockPos);

	// if adjBlock == nullptr means that the block that's being checked to add to the mesh is a block on the edge of the chunk which mesh is being created
	if (adjBlock == nullptr)
	{
		// if the chunk which mesh that's being created has no adjacent chunk, then the edge faces are all rendered
		if (adjChunk == nullptr)
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

Mesh& Mesh::operator= (Mesh& other)
{
	if (this == &other)
		return *this;

	if (&other == NULL)
	{
		return other;
	}

	// TODO: problem: Chunk class has a Mesh field, Mesh class has a Chunk field
	// this fields have their own operator= defined
	// when they are called they cause an infinite loop: in the chunk's operator= the mesh field is assigned an other mesh, this calls the mesh field's operator= that assigns its chunk field, which calls operator= for the mesh, ecc.
	*this->m_Chunk = *other.m_Chunk;

	// assign pointers
	this->m_TerrainBuffers = other.m_TerrainBuffers;

	// assign the values for the offsets because the pointers will be freed when chunks are deleted
	this->m_TerrainOffsets = other.m_TerrainOffsets;

	m_BlocksAddedToMesh.resize(other.m_BlocksAddedToMesh.size());

	for (int i = 0; i < other.m_BlocksAddedToMesh.size(); i++)
	{
		*this->m_BlocksAddedToMesh[i] = *other.m_BlocksAddedToMesh[i];
	}

	return *this;
}

void insertFaceTexCoords(float* coords, unsigned int blockID, int side)
{
	size_t coords_index = 0;

	for (int i = 0; i < INDICES_PER_FACE; i++)
	{
		uint8_t texIndexOffset = 0;
		bool fillVertsClockwise = false;

		if (blockID == grass)
			if (side == top || side == bottom)
				texIndexOffset = 1;

		if (side == back || side == bottom || side == left)
			fillVertsClockwise = true;

		// dimension of the quad for different block textures
		float texQuadDim = 16.0 / 256;

		// base vertex: top-left corner of the face
		float baseVertexX = ((blockID % 16) + texIndexOffset) * texQuadDim;
		float baseVertexY = 1.0 - (blockID / 16) * texQuadDim;
		
		switch (i)
		{
			case 0:
			{
				coords[coords_index] = baseVertexX;
				coords[++coords_index] = baseVertexY;
			} break;
		
			case 1:
			{
				if (!fillVertsClockwise)
				{
					coords[++coords_index] = baseVertexX;
					coords[++coords_index] = baseVertexY - texQuadDim;
				}
				else
				{
					coords[++coords_index] = baseVertexX + texQuadDim;
					coords[++coords_index] = baseVertexY - texQuadDim;
				}
			} break;

			case 2:
			{
				if (!fillVertsClockwise)
				{
					coords[++coords_index] = baseVertexX + texQuadDim;
					coords[++coords_index] = baseVertexY - texQuadDim;
				}
				else
				{
					coords[++coords_index] = baseVertexX;
					coords[++coords_index] = baseVertexY - texQuadDim;
				}
			} break;

			case 3:
			{
				coords[++coords_index] = baseVertexX;
				coords[++coords_index] = baseVertexY;
			} break;

			case 4:
			{
				if (!fillVertsClockwise)
				{
					coords[++coords_index] = baseVertexX + texQuadDim;
					coords[++coords_index] = baseVertexY - texQuadDim;
				}
				else
				{
					coords[++coords_index] = baseVertexX + texQuadDim;
					coords[++coords_index] = baseVertexY;
				}
			} break;

			case 5:
			{
				if (!fillVertsClockwise)
				{
					coords[++coords_index] = baseVertexX + texQuadDim;
					coords[++coords_index] = baseVertexY;
				}
				else
				{
					coords[++coords_index] = baseVertexX + texQuadDim;
					coords[++coords_index] = baseVertexY - texQuadDim;
				}
			} break;
		}
	}
}

//void insertFaceTexCoords(float* coords, unsigned int blockID, int side)
//{
//	size_t coords_index = 0;
//
//	for(int i = 0; i < INDICES_PER_FACE; i++)
//	{
//		// two texture coordinates per index
//		uint8_t texIndexOffset = 0;
//
//		if(blockID == grass)
//			if(side == top || side == bottom)
//				texIndexOffset = 1;
//
//		bool fillVertsClockwise = false;
//
//		if(side == back || side == bottom)
//			fillVertsClockwise = true;
//
//		float texQuadDim = 16.0 / 256;
//
//		// tex coordinates with origin on the top-left corner
//		float texCoordX = texQuadDim * (blockID % 16 + texIndexOffset);
//		float texCoordY = texQuadDim * (blockID / 16);
//
//		// tex coordinates with origin on the bottom-left corner
//		float glTexCoordX = texCoordX;
//		float glTexCoordY = 1.0 - texCoordY; // evaluates to 1.0 for the first row of the atlas, then + 1 for the next rows
//
//		switch (i)
//		{
//		// vertex 0, tex coordinate corresponding to vertex 0: bottom left of the face
//		case 0: {
//			coords[coords_index] = glTexCoordX;
//			coords[++coords_index] = glTexCoordY - texQuadDim;
//		}; break;
//		case 1: {
//			if (fillVertsClockwise) {
//				coords[coords_index] = glTexCoordX;
//				coords[++coords_index] = glTexCoordY;
//			}
//			else
//			{
//				coords[coords_index] = glTexCoordX + texQuadDim;
//				coords[++coords_index] = glTexCoordY - texQuadDim;
//			}
//		}; break;
//		case 2: {
//			if (fillVertsClockwise) {
//				coords[coords_index] = glTexCoordX + texQuadDim;
//				coords[++coords_index] = glTexCoordY - texQuadDim;
//			}
//			else
//			{
//				coords[coords_index] = glTexCoordX;
//				coords[++coords_index] = glTexCoordY;
//			}
//		}; break;
//		case 3: {
//			coords[coords_index] = glTexCoordX;
//			coords[++coords_index] = glTexCoordY;
//
//		}; break;
//		case 4: {
//			if (fillVertsClockwise) {
//				coords[coords_index] = glTexCoordX + texQuadDim;
//				coords[++coords_index] = glTexCoordY;
//			}
//			else
//			{
//				coords[coords_index] = glTexCoordX + texQuadDim;
//				coords[++coords_index] = glTexCoordY - texQuadDim;
//			}
//		}; break;
//		case 5: {
//			if (fillVertsClockwise) {
//				coords[coords_index] = glTexCoordX + texQuadDim;
//				coords[++coords_index] = glTexCoordY - texQuadDim;
//			}
//			else
//			{
//				coords[coords_index] = glTexCoordX + texQuadDim;
//				coords[++coords_index] = glTexCoordY;
//			}
//		} break;
//		}
//
//		coords_index++;
//	}
//}

//mat4 transform_face(Camera* camera, float* face, int side, vec3 blockWorldPos)
//{
//	vec3 translation = vec3(0.0, 0.0, 0.0);
//	float angle = 0.0;
//
//	angle = to_radians(angle);
//
//	mat4 rot_mat = mat4(1.0);
//
//	switch (side)
//	{
//	case top: {
//		translation = vec3(0.0, 1.0, 0.0);
//		angle = to_radians(-90.0);
//
//		rot_mat = mat4(
//			1, 0, 0, 0,
//			0, cos(angle), sin(angle), 0,
//			0, -sin(angle), cos(angle), 0,
//			0, 0, 0, 1
//		);
//	} break;
//
//	case right: {
//		translation = vec3(1.0, 0.0, 0.0);
//		angle = to_radians(90.0);
//
//		rot_mat = mat4(
//			cos(angle), 0, -sin(angle), 0,
//			0, 1, 0, 0,
//			sin(angle), 0, cos(angle), 0,
//			0, 0, 0, 1
//		);
//	} break;
//
//	case left: {
//		angle = to_radians(90.0);
//
//		rot_mat = mat4(
//			cos(angle), 0, -sin(angle), 0,
//			0, 1, 0, 0,
//			sin(angle), 0, cos(angle), 0,
//			0, 0, 0, 1
//		);
//	} break;
//
//	case bottom: {
//		angle = to_radians(-90.0);
//
//		rot_mat = mat4(
//			1, 0, 0, 0,
//			0, cos(angle), sin(angle), 0,
//			0, -sin(angle), cos(angle), 0,
//			0, 0, 0, 1
//		);
//	} break;
//
//	case back: {
//		translation = vec3(0.0, 0.0, -1.0);
//	} break;
//	}
//
//	mat4 model = mat4(1.0);
//
//	// these next steps are like multiplicating rotation and translation matrices together
//	// normally, meaning with matrices used in row-major order, the order of multiplication is rotation * translation to get the correct transformation
//	// here the order is inverted to get the same result because glm::mats are stored in colum-major order
//	model = glm::translate(model, blockWorldPos);
//	model = glm::translate(model, translation);
//	//model = glm::rotate(model, glm::radians(angle), rot_axis);
//	model = model * rot_mat;
//
//	// complete the mvp with view and perspective transformations
//	mat4 view = camera->GetViewMat4();
//
//	mat4 projection = camera->GetProjectionMat4();
//
//	mat4 transform_mat = projection * view * model;
//
//	return transform_mat;
//}

// 1. ruota faccia 2. trasla faccia 3. trasla blocco

//// side: 6 different directions of the block faces
//void Mesh::AddFaceToMesh(Block* block, uint8_t side)
//{
//	//mat4 mvp = transform_face(m_Player->GetCam(), face, side, block->GetWorldPosition());
//
//	mat4 model = m_Player->GetCam()->GetModelMat4(block->GetWorldPosition(), side);
//	//mat4 view = m_Player->GetCam()->GetViewMat4();
//	//mat4 projection = m_Player->GetCam()->GetProjectionMat4();
//
//	// turn those matrices into float arrays, then insert them in the vectors
//	float model_arr[16], view_arr[16], proj_arr[16];
//
//	mat4_to_float(model, model_arr);
//	//mat4_to_float(view, view_arr);
//	//mat4_to_float(projection, proj_arr);
//
//	float texCoords[VALUES_PER_TEX_COORD * INDICES_PER_FACE];
//
//	insertFaceTexCoords(texCoords, block->GetID(), side);
//
//	m_ModelMats.insert(m_ModelMats.end(), model_arr, model_arr + 16);
//	//m_ViewMats.insert(m_ViewMats.end(), view_arr, view_arr + 16);
//	//m_ProjMats.insert(m_ProjMats.end(), proj_arr, proj_arr + 16);
//	m_TexCoords.insert(m_TexCoords.end(), texCoords, texCoords + std::size(texCoords));
//}