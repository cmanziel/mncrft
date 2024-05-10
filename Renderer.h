#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Terrain.h"
#include "Shader.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
public:
	Renderer();
	void Draw(Terrain* terrain, Player* player, int chunkNum);
	~Renderer();
private:
	Shader* m_Shader;
	Buffer* m_FaceBuffers[FACES_PER_BLOCK];
};

#endif // !RENDERER_H