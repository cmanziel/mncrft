#include "Renderer.h"

//float backFace[] =
//{
//	0, 0, 0,
//	0, 1, 0,
//	1, 0, 0,
//	0, 1, 0,
//	1, 1, 0,
//	1, 0, 0
//};
//
//float frontFace[] =
//{
//	0, 0, 1,
//	1, 0, 1,
//	0, 1, 1,
//	0, 1, 1,
//	1, 0, 1,
//	1, 1, 1
//};
//
//float leftFace[] =
//{
//	0, 0, 0,
//	0, 0, 1,
//	0, 1, 0,
//	0, 1, 0,
//	0, 0, 1,
//	0, 1, 1
//};
//
//float rightFace[] =
//{
//	1, 0, 1,
//	1, 0, 0,
//	1, 1, 1,
//	1, 1, 1,
//	1, 0, 0,
//	1, 1, 0
//};
//
//float topFace[] =
//{
//	0, 1, 1,
//	1, 1, 1,
//	0, 1, 0,
//	0, 1, 0,
//	1, 1, 1,
//	1, 1, 0
//};
//
//float bottomFace[] =
//{
//	0, 0, 1,
//	0, 0, 0,
//	1, 0, 1,
//	0, 0, 0,
//	1, 0, 0,
//	1, 0, 1
//};

float backFace[] =
{
	0.0, 1.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 0.0, 0.0
};

float frontFace[] =
{
	0.0, 1.0, 1.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 1.0
};

float leftFace[] =
{
	0.0, 1.0, 1.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0
};

float rightFace[] =
{
	1.0, 1.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 1.0, 1.0,
	1.0, 0.0, 0.0,
	1.0, 1.0, 0.0
};

float topFace[] =
{
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	0.0, 1.0, 0.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 0.0
};

float bottomFace[] =
{
	0.0, 0.0, 0.0,
	1.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 1.0
};

// array of six float pointers, one for each face
float* faces[FACES_PER_BLOCK] = { topFace, bottomFace, leftFace, rightFace, frontFace, backFace };

Renderer::Renderer()
{
	// do the vertex specification for every face here
	for (int i = 0; i < FACES_PER_BLOCK; i++)
	{
		m_FaceBuffers[i] = new Buffer(faces[i], INDICES_PER_FACE * VALUES_PER_VERTEX * sizeof(float));

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, VALUES_PER_VERTEX, GL_FLOAT, GL_FALSE, VALUES_PER_VERTEX * sizeof(float), (void*)0);
	}

	m_Shader = new Shader("shaders/shader.vs", "shaders/shader.fs");
	m_Shader->CreateProgram();
}


void Renderer::Draw(Terrain* terrain, Player* player)
{
	// for every chunk send to the shader the relative texture coordinates and mvps (colum by column)
	for (Chunk* chunk : terrain->m_Chunks)
	{
		Mesh* mesh = chunk->GetMesh();
		terrain_buffers* terrain_bufs = mesh->GetTerrainBufs();
		//offsets* buffers_offsets = mesh->GetTerrainOffsets();
		offsets buffer_offsets = mesh->GetTerrainOffsets();
		
		terrain_bufs->face_index->Bind();

		glEnableVertexAttribArray(16);
		//glVertexAttribIPointer(16, 1, GL_INT, sizeof(int), (void*)buffers_offsets->face_index);
		glVertexAttribIPointer(16, 1, GL_INT, sizeof(int), (void*)buffer_offsets.face_index);
		glVertexAttribDivisor(16, 1);

		terrain_bufs->tex->Bind();

		for (int i = 6; i < INDICES_PER_FACE + 6; i++)
		{
			glEnableVertexAttribArray(i);
			// set of 2 * INDICES_PER_FACE vertices for every face are passed to the shader
			//glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(buffers_offsets->tex + (i - 6) * 2 * sizeof(float)));
			glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(buffer_offsets.tex + (i - 6) * 2 * sizeof(float)));
			glVertexAttribDivisor(i, 1);
		}

		// pass the model and projection matrices to the shaders via a uniform instead of as a vertex attribute
		mat4 view = player->GetCam()->GetViewMat4();
		mat4 proj = player->GetCam()->GetProjectionMat4();

		int model_attrib_index = 11;

		terrain_bufs->model->Bind();
		
		glEnableVertexAttribArray(++model_attrib_index);
		//glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)buffers_offsets->model);
		glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)buffer_offsets.model);
		glVertexAttribDivisor(model_attrib_index, 1);

		glEnableVertexAttribArray(++model_attrib_index);
		//glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(buffers_offsets->model + 4 * sizeof(float)));
		glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(buffer_offsets.model + 4 * sizeof(float)));
		glVertexAttribDivisor(model_attrib_index, 1);

		glEnableVertexAttribArray(++model_attrib_index);
		//glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(buffers_offsets->model + 8 * sizeof(float)));
		glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(buffer_offsets.model + 8 * sizeof(float)));
		glVertexAttribDivisor(model_attrib_index, 1);

		glEnableVertexAttribArray(++model_attrib_index);
		//glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(buffers_offsets->model + 12 * sizeof(float)));
		glVertexAttribPointer(model_attrib_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(buffer_offsets.model + 12 * sizeof(float)));
		glVertexAttribDivisor(model_attrib_index, 1);

		m_Shader->use();
		glUniform1i(glGetUniformLocation(m_Shader->get_id(), "tex"), 0);

		m_Shader->setUniformMatrix4fv("view", glm::value_ptr(view));
		m_Shader->setUniformMatrix4fv("proj", glm::value_ptr(proj));

		glDrawArraysInstanced(GL_TRIANGLES, 0, INDICES_PER_FACE, mesh->GetFacesIndex().size());
	}
}

Renderer::~Renderer() {
	delete m_Shader;

	for (int i = 0; i < FACES_PER_BLOCK; i++)
	{
		delete m_FaceBuffers[i];
	}
}