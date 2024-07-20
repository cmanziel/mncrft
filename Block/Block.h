#pragma once

#include "../Buffer/Buffer.h"
#include "../utils/glm_types.h"

#define VALUES_PER_FACE 18
#define INDICES_PER_FACE 6
#define FACES_PER_BLOCK 6
#define VALUES_PER_VERTEX 6 // 3 positions coordinates and 3 barycentric
#define VALUES_PER_COORD 3
#define VALUES_PER_TEX_COORD 2

enum sides {
	top, bottom, left, right, front, back
};

enum BlockID {
	air = -1, grass, dirt = 2, cobblestone
};

class Block
{
public:
	Block(vec3 localPos, vec3 worldPos, short ID);
	Block(Block& other);
	~Block();

	Block& operator=(Block& other);

	vec3 GetWorldPosition();
	vec3 GetLocalPosition();

	short GetID();
	void SetID(short ID);

	bool m_IsPointed; // flag to determine if block is pointed by camera ray or not

private:
	vec3 m_LocalPosition; // position in the chunk
	vec3 m_WorldPosition; // position in the 3d world
	short m_ID;
};
