#include "vector3.h"
#include <math.h>
#include "vector_functions.h"

using namespace std;

float magnitude(vector3 const& vec) {
	return sqrt(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
}

vector3 normalize(vector3 vec) {
	return vec / magnitude(vec);
}

float dot(vector3 const& a, vector3 const& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float clamp(float value, float min, float max) {
	return fmax(fmin(value, max), min);
}
