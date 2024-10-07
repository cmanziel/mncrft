#include "NoiseMap.h"

#define PI 3.14

void rotate_domain(float* x, float* y, float angle)
{
	float xt = *x * cos(angle) - *y * sin(angle);

	*y = *x * sin(angle) + *y * cos(angle);
	*x = xt;
}

float NoiseMap::GetValue(float x, float z, unsigned long seed)
{
	float nv = noise2((x + seed) / 512.0f, (z + seed) / 512.0f);

	rotate_domain(&x, &z, PI / 4);
	nv += 0.5f * noise2((x + seed) / 256.0f, (z + seed) / 256.0f);
	rotate_domain(&x, &z, PI / 4);
	nv += 0.25f * noise2((x + seed) / 128.0f, (z + seed) / 128.0f);

	nv /= 1.75f;

	nv = powf(fabsf(nv), 1.2f);

	nv += 1.0f;
	nv /= 2.0f;

	return nv;
}