#pragma once
#include "vector3.h"
#include "models.h"

float generateNoise(int x, int y);

bool sphereIntersect(
	vector3& origin,
	vector3& rayDir,
	SceneObject& obj,
	float& outT
);

bool ringIntersect(
	vector3& origin,
	vector3& rayDir,
	SceneObject& obj,
	float& outT
);
