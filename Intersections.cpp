#include <math.h>
#include "vector3.h";
#include "vector_functions.h"
#include "intersaction_functions.h"
#include <cmath>
#include <stdint.h>

const float EPS = 0.0001f;

float generateNoise(int x, int y) {
	uint32_t n = x + y * 57;
	n = (n << 13) ^ n;
	// Classic Wang hash variation
	uint32_t hash = (n * (n * n * 15731 + 789221) + 1376312589);
	// Normalize to [0, 1)
	return static_cast<float>(hash & 0x7fffffff) / 2147483648.0f;
}

bool sphereIntersect(
	vector3& origin,
	vector3& rayDir,
	SceneObject& obj,
	float& outT
) {
	Sphere* s = static_cast<Sphere*>(&obj);
	if (!s)
		return false;

	Sphere sphere = *s;
	vector3 centre = sphere.getPosition();
	float radius = sphere.getRadius();

	vector3 oc = origin - centre;

	float a = dot(rayDir, rayDir);
	float b = 2.0f * dot(oc, rayDir);
	float c = dot(oc, oc) - radius * radius;

	//дискриминант
	float d = b * b - 4.0f * a * c;
	if (d < 0)
		return false;

	float sqrtD = sqrt(d);

	// вход и выход луча из сферы
	float t0 = (-b - sqrtD) / (2.0f * a);
	float t1 = (-b + sqrtD) / (2.0f * a);

	// берём ближайший положительный
	float t = t0;
	if (t < EPS) t = t1;
	if (t < EPS) return false;

	outT = t;
	return true;
}

bool ringIntersect(vector3& origin,
	vector3& rayDir,
	SceneObject& obj,
	float& outT) {

	Ring* ring = dynamic_cast<Ring*>(&obj);
	if (ring == nullptr)
		return false;

	//не параллелен ли луч плоскости
	float denom = dot(rayDir, ring->getNormal());

	if (abs(denom) < EPS)
		return false;

	vector3 toPlane = ring->getPosition() - origin;
	float t = dot(toPlane, ring->getNormal()) / denom;

	if (t <= EPS)
		return false;

	//точка пересечения с плоскостью
	vector3 hitPoint = origin + rayDir * t;

	//найти вектор от центра кольца до точки
	vector3 fromCenter = hitPoint - ring->getPosition();

	float dist = magnitude(fromCenter);

	if (dist < ring->getInner())
		return false;
	if (dist > ring->getOuter())
		return false;

	outT = t;
	return true;
}