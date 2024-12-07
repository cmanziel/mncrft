#include "NoiseMap.h"

using namespace noise;

#define PI 3.14

#define sign(_x) ({ __typeof__(_x) _xx = (_x);\
    ((__typeof__(_x)) ( (((__typeof__(_x)) 0) < _xx) - (_xx < ((__typeof__(_x)) 0))));})

void rotate_domain(float* x, float* y, float angle)
{
	float xt = *x * cos(angle) - *y * sin(angle);

	*y = *x * sin(angle) + *y * cos(angle);
	*x = xt;
}

float NoiseMap::octave(unsigned int n, unsigned int o, float x, float z, int seed)
{
	float u = 1.0f, v = 0.0f;

	for (int i = 0; i < n; i++)
	{
		v += (1.0f / u) * noise3((x / 1.01f) * u, (z / 1.01f), seed + o * 16);
		u *= 2.0f;
	}

	return v;
}

float NoiseMap::GetValue(float x, float z, int seed)
{
	float o1 = octave(5, 2, x / 512.0f, z / 512.0f, seed);

	float cs = octave(7, 1, x / 512.0f + o1, z / 512.0f, seed) * 0.5f + 0.5f;
	
	//if (cs < 0.5f)
	//	cs = powf(cs, 3.5f);

	cs = powf(cs, 3.0f);

	return cs;

	//float o1 = octave(5, 2, x / 512.0f, z / 512.0f, seed) * 0.5f + 0.5f;

	//o1 = powf(o1, 3.0f);

	//return o1;

	//float flatNoise = noise3(x / 512.0f, z / 512.0f, seed) * 0.05f - 0.99f;
	//float mountainNoise = 0.5f * noise3(x / 256.0f, z / 256.0f, seed);

	//float nv = flatNoise + mountainNoise / 1.5f;

	//if (nv < -1.0f)
	//	nv = -1.0f;

	//if (nv > 1.0f)
	//	nv = 1.0f;

	//return nv * 0.5f + 0.5f;
}