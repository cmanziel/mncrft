#pragma once

#include <vector>
#include <GL/glew.h>

class Buffer
{
public:
	Buffer(std::vector<float> data, int size);
	Buffer(float* data, int size);
	Buffer(int* data, int size);
	~Buffer();
	
	void Bind();
	void Unbind();

	void SubData(float* data, unsigned int size, int offset);
	void SubData_int(int* data, unsigned int size, int offset);

	std::vector<float> GetData();
	unsigned int GetID();
	unsigned int GetSizeAllocated();

private:
	unsigned int m_ID;
	std::vector<float> m_Data;
	unsigned int m_SizeAllocated;
};