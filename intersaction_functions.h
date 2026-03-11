#pragma once
#include "vector3.h"
#include "models.h"

float generateNoise(int x, int y);

bool sphereIntersect(
	vector3 const& origin,
	vector3 const& rayDir,
	Sphere const& sphere,
	float& outT
);

bool ringIntersect(
	vector3 const& origin,
	vector3 const& rayDir,
	Ring const& ring,
	float& outT
);

bool shadowIntersect(
	vector3 const& hitPoint,
	vector3 const& normal,
	vector3 const& lightPos,
	Sphere* spheres,
	const int size,
	Ring const& ring
);

enum HitType {
	None,
	RingType,
	SphereType
};