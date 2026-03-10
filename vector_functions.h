#pragma once
#include "vector3.h"
#include<math.h>

float magnitude(vector3 const& vec);
vector3 normalize(vector3 vec);
float dot(vector3 const& a, vector3 const& b);
float clamp(float value, float min, float max);