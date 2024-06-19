#include "Ray.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	m_Origin = origin;

	m_Direction = glm::normalize(direction);
}

point Ray::Origin() const
{
	return m_Origin;
}

vec3 Ray::Direction() const
{
	return m_Direction;
}

// find the point that's distant t from the origin in the ray's direction
point Ray::at(float t) const
{
	return m_Origin + t * m_Direction;
}

// origin(X0): a plane can determined by a normal vector and a point: origin
float Ray::point_plane_distance(point origin, vec3 plane_normal, point hit_point)
{
	float a = plane_normal.x;
	float b = plane_normal.y;
	float c = plane_normal.z;

	float d = -(a * origin.x + b * origin.y + c * origin.z);
	float dist = abs((long)(a * hit_point.x + b * hit_point.y + c * hit_point.z + d)) / sqrt(a * a + b * b + c * c);

	return dist;
}