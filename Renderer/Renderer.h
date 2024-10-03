#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include "../Terrain/Terrain.h"
#include "../Shader/Shader.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
public:
	Renderer();
	void Draw(Terrain* terrain);
	~Renderer();
private:
	Shader* m_Shader;
	Buffer* m_FaceBuffers[FACES_PER_BLOCK];
};

#endif // !RENDERER_H