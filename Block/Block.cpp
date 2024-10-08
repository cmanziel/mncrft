#include "Block.h"

Block::Block(vec3 localPos, vec3 worldPos, short ID)
    : m_LocalPosition(localPos), m_WorldPosition(worldPos), m_ID(ID), m_IsPointed(false)
{

}

Block::Block(Block& other)
{
    m_LocalPosition = other.m_LocalPosition;
    m_WorldPosition = other.m_WorldPosition;
    m_ID = other.m_ID;
    m_IsPointed = other.m_IsPointed;
}

Block& Block::operator=(Block& other)
{
    if (this == &other)
        return *this;

    m_LocalPosition = other.m_LocalPosition;
    m_WorldPosition = other.m_WorldPosition;

    m_ID = other.m_ID;

    m_IsPointed = other.m_IsPointed;
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

void Block::SetID(short ID)
{
    m_ID = ID;
}

// order in which vertices are inserted in faces for correct culling
// counter-clockwise for back, bottom and right faces
// clockwise the other ones