#include "Renderer.h"

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

	m_Shader = new Shader("Shader/shaders/shader.vs", "Shader/shaders/shader.fs");
	m_Shader->CreateProgram();
}

void Renderer::Draw(Terrain* terrain)
{
	for (int z = 0; z < CHUNK_RADIUS * 2 + 1; z++)
	{
		for (int x = 0; x < CHUNK_RADIUS * 2 + 1; x++)
		{
			Chunk* chunk = terrain->m_Chunks[z][x];
			Mesh* mesh = chunk->GetMesh();

			terrain_buffers* terrain_bufs = mesh->GetTerrainBufs();

			// skip to the next chunk if the current one doesn't have vertices to be drawn this frame
			if (terrain_bufs == NULL)
				continue;

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

			// pass the model and projection matrices to the shaders via a uniform instead of as a vertex attribute
			mat4 view = chunk->GetPlayer()->GetCam()->GetViewMat4();
			mat4 proj = chunk->GetPlayer()->GetCam()->GetProjectionMat4();

			m_Shader->setUniformMatrix4fv("view", glm::value_ptr(view));
			m_Shader->setUniformMatrix4fv("proj", glm::value_ptr(proj));

			glDrawArraysInstanced(GL_TRIANGLES, 0, INDICES_PER_FACE, mesh->GetFacesIndex().size());
		}
	}
}

Renderer::~Renderer() {
	delete m_Shader;

	for (int i = 0; i < FACES_PER_BLOCK; i++)
	{
		delete m_FaceBuffers[i];
	}
}
