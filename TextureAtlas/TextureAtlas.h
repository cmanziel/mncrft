#pragma once
#ifndef TEXATLAS_H
#define TEXATLAS_H

#include <GL/glew.h>

class TextureAtlas
{
public:
	TextureAtlas();
	~TextureAtlas();

	unsigned int GetID();
	void Bind();

private:
	unsigned int m_ID;
};

#endif // !TEXATLAS_H