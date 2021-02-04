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

struct myVertex4
{
	float  x;
	float  y;
	float  z;
	float  w;
	myVertex4() {}
	myVertex4(float x, float y, float z, float  w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

struct myVertex2
{
	float  x;
	float  y;

	myVertex2() {}
	myVertex2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};