#pragma once

#include <float.h>

#include "../../def.h"

const uint SPHERE = 1;
const uint PLANE = 2;

struct descr
{
	vec3 center;
	float r;
	vec3 N;
	float D;
};


struct shape
{
	vec3 color;
	uint type;
	descr data;
};


#if 0
class shape {
public:
	vec3 pos;
	color Color;

	shape() {}
	virtual ~shape() {}
	virtual dbl sdf(vec3 p) {
		return DBL_MAX;
	}
};


class sphere : public shape {
public:
	dbl radius;

	sphere(dbl sradius, vec3 spos)
	{
		radius = sradius;
		pos = spos;
		Color = color(255, 0, 0);
	}

	~sphere() {}

	dbl sdf(vec3 p)
	{
		return length(pos - p) - radius;
	}
};

class box : public shape {
public:
	vec3 sides; // box sides values

	box(vec3 bsides, vec3 bpos)
	{
		sides = bsides;
		pos = bpos;
		Color = color(255, 0, 0);
	}

	~box() {}
	
	dbl sdf(vec3 p)
	{
		p = p - pos;
		vec3 q = abs(p) - sides;
		return length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);
	}
};


class torus : public shape {
public:
	vec2 Rr; // inner and outer radiuses

	torus(vec2 tRr, vec3 tpos)
	{
		Rr = tRr;
		pos = tpos;
		Color = color(255, 0, 0);
	}

	~torus() {}

	dbl sdf(vec3 p)
	{
		p = p - pos;
		vec2 q = vec2(length(vec2(p.x, p.z)) - Rr.x, p.y);
		return length(q) - Rr.y;
	}
};

class plane : public shape {
public:
	vec3 N; // plane normal

	plane(vec3 pN, vec3 ppos)
	{
		N = pN;
		pos = ppos;
		Color = color(255, 0, 0);
	}

	~plane() {}

	dbl sdf(vec3 p)
	{
		p = p - pos;
		return dot(N, p);
	}
};
#endif