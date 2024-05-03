#include "Buffer.h"

Buffer::Buffer(float* data, int size)
{
	glGenBuffers(1, &m_ID);
	Bind();

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	m_SizeAllocated = size;
}

Buffer::Buffer(int* data, int size)
{
	glGenBuffers(1, &m_ID);
	Bind();

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	m_SizeAllocated = size;
}

void Buffer::SubData(float* data, unsigned int size, int offset)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void Buffer::SubData_int(int* data, unsigned int size, int offset)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void Buffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void Buffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int Buffer::GetID()
{
	return m_ID;
}

unsigned int Buffer::GetSizeAllocated()
{
	return m_SizeAllocated;
}

std::vector<float> Buffer::GetData()
{
	return m_Data;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_ID);
}