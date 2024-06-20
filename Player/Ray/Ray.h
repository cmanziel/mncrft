#ifndef RAY_H
#define RAY_H

#include "../../utils/glm_types.h"

typedef vec3 point;


class Ray
{
public:
	Ray();
	Ray(vec3 origin, vec3 dir);

	point Origin() const;
	vec3 Direction() const;

	static float point_plane_distance(point plane_origin, vec3 plane_normal, point hit_point);

	point at(float t) const;

private:
	point m_Origin;
	vec3 m_Direction;
};

#endif