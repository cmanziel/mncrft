#include "Mesh.h"
#include <glm/ext/matrix_transform.hpp>
#include <vector>

// blockAddedToMesh field stores the blocks that have a face which vertices are added to the mesh
// in the renderer iterate through the blockAddedToMesh field set the MVP for the current vertices

void insertFaceTexCoords(float* coords, unsigned int blockID, int side);
//mat4 transform_face(Camera* camera, float* face, int side, vec3 blockWorldPos);

//enum sides {
//	front, back, left, right, top, bottom
//};

Mesh::Mesh()
{
	m_TerrainBuffers = NULL;
}

Mesh::~Mesh()
{
	Clear();
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

void Mesh::Clear()
{
	m_ModelMats.clear();
	m_TexCoords.clear();
	m_Faces.clear();
	m_PointedFlags.clear();
}

void Mesh::Build(terrain_buffers* terrainBufs, offsets terrainBuffersOffsets)
{
	if (m_TerrainBuffers == NULL)
		m_TerrainBuffers = terrainBufs;

	m_TerrainOffsets = terrainBuffersOffsets;

	m_TerrainBuffers->model->SubData(m_ModelMats.data(), m_ModelMats.size() * sizeof(float), m_TerrainOffsets.model);
	m_TerrainBuffers->tex->SubData(m_TexCoords.data(), m_TexCoords.size() * sizeof(float), m_TerrainOffsets.tex);
	m_TerrainBuffers->face_index->SubData_int(m_Faces.data(), m_Faces.size() * sizeof(int), m_TerrainOffsets.face_index);
	m_TerrainBuffers->pointedFlags->SubData(m_PointedFlags.data(), m_PointedFlags.size() * sizeof(float), m_TerrainOffsets.pointedFlags);
}

// add to the mesh the faces checked by the Chunk's class methods
void Mesh::AddFace(Block* block, Camera* cam, uint8_t side)
{
	mat4 model = cam->GetModelMat4(block->GetWorldPosition(), side);

	float model_arr[16];

	mat4_to_float(model, model_arr);

	float texCoords[VALUES_PER_TEX_COORD * INDICES_PER_FACE];

	insertFaceTexCoords(texCoords, block->GetID(), side);

	m_Faces.push_back(side);
	m_ModelMats.insert(m_ModelMats.end(), model_arr, model_arr + 16);
	m_TexCoords.insert(m_TexCoords.end(), texCoords, texCoords + std::size(texCoords));

	if (block->m_IsPointed)
		m_PointedFlags.push_back(1.0);
	else
		m_PointedFlags.push_back(0.0);
}

Mesh& Mesh::operator= (Mesh& other)
{
	if (this == &other)
		return *this;

	if (&other == NULL)
	{
		return other;
	}

	// assign pointers
	this->m_TerrainBuffers = other.m_TerrainBuffers;

	// assign the values for the offsets because the pointers will be freed when chunks are deleted
	this->m_TerrainOffsets = other.m_TerrainOffsets;

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
		{
			if (side == top)
				texIndexOffset = 1;
			else if (side == bottom)
				blockID = dirt; // apply a dirt texture to the bottom face of a grass block
		}

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