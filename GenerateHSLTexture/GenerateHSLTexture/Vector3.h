#ifndef _VECTOR3_H_
#define _VECTOR3_H_

struct vec3
{
	float r;
	float g;
	float b;
	vec3()
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
	}
	vec3(float r1, float g1, float b1)
	{
		r = r1;
		g = g1;
		b = b1;
	}
};

#endif