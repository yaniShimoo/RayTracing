#pragma once
#include <math.h>

class vector3 {
public:
	float x, y, z;

	vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	const vector3& operator + (vector3 const& coll) const {
		return vector3(x + coll.x, y + coll.y, z + coll.z);
	}
	const vector3& operator - (vector3 const& coll) const {
		return vector3(x - coll.x, y - coll.y, z - coll.z);
	}
	vector3 operator * (vector3 const& coll) {
		return vector3(x * coll.x, y * coll.y, z * coll.z);
	}
	const vector3& operator * (float coll) const {
		return vector3(x * coll, y * coll, z * coll);
	}
	vector3 operator / (vector3 const& coll) {
		return vector3(x / coll.x, y / coll.y, z / coll.z);
	}
	vector3 operator/ (float coll) {
		return vector3(x / coll, y / coll, z / coll);
	}
};
