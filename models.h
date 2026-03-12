#pragma once
#include "vector3.h"

class SceneObject {
protected:
	vector3 pos;
public:
	const vector3& getPosition() const {
		return pos;
	}

	void setPosition(vector3 const& vec) {
		pos = vec;
	}

	virtual ~SceneObject() {}
};



class Ring : public SceneObject
{
private:
	float innerRadius;
	float outerRadius;
	vector3 normal;
public:
	Ring(float innerRadius, float outerRadius, vector3 const& normal, vector3 const& pos) {
		this->innerRadius = innerRadius;
		this->outerRadius = outerRadius;
		this->normal = normal;
		this->pos = pos;
	}
	const vector3& getNormal() const {
		return normal;
	}
	const float& getInner() const {
		return innerRadius;
	}
	const float& getOuter() const {
		return outerRadius;
	}
};

class Sphere : public SceneObject {
private:
	float radius;

	float orbitRadius;
	float orbitSpeed;

	float orbitTiltX;
	float orbitTiltY;
	float orbitTiltZ;
public:

	Sphere(float radius, vector3 const& pos) {

		this->radius = radius;
		this->pos = pos;

		orbitRadius = 0;
		orbitSpeed = 0;
		orbitTiltX = 0;
		orbitTiltY = 0;
		orbitTiltZ = 0;
	}
	Sphere(float radius, vector3 const& pos,
		float orbitRadius,
		float orbitSpeed) {

		this->radius = radius;
		this->pos = pos;
		this->orbitRadius = orbitRadius;
		this->orbitSpeed = orbitSpeed;
		this->orbitTiltX = 0;
		this->orbitTiltY = 0;
		this->orbitTiltZ = 0;
	}
	Sphere(float radius, vector3 const& pos,
		float orbitRadius,
		float orbitSpeed,
		float orbitTiltX,
		float orbitTiltY,
		float orbitTiltZ) {

		this->radius = radius;
		this->pos = pos;
		this->orbitRadius = orbitRadius;
		this->orbitSpeed = orbitSpeed;
		this->orbitTiltX = orbitTiltX;
		this->orbitTiltY = orbitTiltY;
		this->orbitTiltZ = orbitTiltZ;

	}
	const float& getRadius() const {
		return radius;
	}
	void setRadius(float radius) {
		this->radius = radius;
	}
	const float& getOrbitSpeed() const {
		return orbitSpeed;
	}
	const float& getTiltX() const {
		return orbitTiltX;
	}
	const float& getTiltY() const {
		return orbitTiltY;
	}
	const float& getTiltZ() const {
		return orbitTiltZ;
	}
};

struct MoonData
{
	float orbitRadius;
	float orbitSpeed;

	float orbitTiltX;
	float orbitTiltY;
	float orbitTiltZ;

	float radius;
};
