#pragma once
#include <cmath>

#define PI 3.141592654f
#define EPSILON 0.00001f
#define PI2M (PI * 2)
#define PI2D		float(PI / 2.0f)
#define PI4D		float(PI / 4.0f)
#define PI8D		float(PI / 8.0f)
#define PI16D		float(PI / 16.0f)
#define PI32D		float(PI / 32.0f)
#define PI64D		float(PI / 64.0f)
#define PI128D		float(PI / 128.0f)
#define RA_TO_DE(p) 180.0f / PI * (p)
#define DE_TO_RA(p) PI / 180.0f * (p)
#define FLOAT_TO_INT(p) static_cast<int>(p + EPSILON)
#define FLOAT_EQUAL(f1, f2) (fabs(f1 - f2) <= EPSILON)