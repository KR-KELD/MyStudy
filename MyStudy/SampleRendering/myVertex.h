#pragma once
#include "myStd.h"

struct myVertex3
{
	float  x;
	float  y;
	float  z;
	myVertex3() {}
	myVertex3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};