#include "Block.h"

Block::Block(vec3 localPos, vec3 worldPos, short ID)
    : m_LocalPosition(localPos), m_WorldPosition(worldPos), m_ID(ID)
{

}

Block::Block(Block& other)
{
    m_LocalPosition = other.m_LocalPosition;
    m_WorldPosition = other.m_WorldPosition;
    m_ID = other.m_ID;
}

Block& Block::operator=(Block& other)
{
    if (this == &other)
        return *this;

    m_LocalPosition = other.m_LocalPosition;
    m_WorldPosition = other.m_WorldPosition;

    m_ID = other.m_ID;
}

Block::~Block()
{

}

vec3 Block::GetWorldPosition() {
    return m_WorldPosition;
}

vec3 Block::GetLocalPosition() {
    return m_LocalPosition;
}

short Block::GetID() {
    return m_ID;
}

// order in which vertices are inserted in faces for correct culling
// counter-clockwise for back, bottom and right faces
// clockwise the other ones