#pragma once
#include <cmath>

static const float PI = 3.141592654f;
static const float EPSILON = 0.00001f;
static const float PI2M = PI * 2;
static const float PI2D = float(PI / 2.0f);
static const float PI4D = float(PI / 2.0f);
static const float PI8D = float(PI / 2.0f);
static const float PI16D = float(PI / 2.0f);
static const float PI32D = float(PI / 2.0f);
static const float PI64D = float(PI / 2.0f);
static const float PI128D = float(PI / 2.0f);

static float RA_TO_DE(float f)
{
	return (180.0f / PI * f);
}

static float DE_TO_RA(float f)
{
	return (PI / 180.0f * f);
}

static int FLOAT_TO_INT(float f)
{
	return static_cast<int>(f + EPSILON);
}

static bool FLOAT_EQUAL(float f1, float f2)
{
	if (fabs(f1 - f2) <= EPSILON)
		return true;
	return false;
}

struct float2
{
	union
	{
		struct { float x, y; };
		float v[2];
	};
};

struct myPoint : float2
{
	myPoint() = default;
	myPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	float& operator [] (int index)
	{
		if (index == 0) return x;
		return y;
	}
	float* get()
	{
		return v;
	}
	void operator = (float* arr)
	{
		x = arr[0];
		y = arr[1];
	}

	bool operator == (myPoint& p)
	{
		if (FLOAT_EQUAL(x, p.x))
		{
			if (FLOAT_EQUAL(y, p.y))
			{
				return true;
			}
		}
		return false;
	}
	bool operator != (myPoint& p)
	{
		if (FLOAT_EQUAL(x, p.x))
		{
			if (FLOAT_EQUAL(y, p.y))
			{
				return false;
			}
		}
		return true;
	}
	myPoint operator + (myPoint p)
	{
		myPoint pt;
		pt.x = x + p.x;
		pt.y = y + p.y;
		return pt;
	}
	myPoint operator + (float fValue)
	{
		myPoint pt;
		pt.x = x + fValue;
		pt.y = y + fValue;
		return pt;
	}
	myPoint operator - (myPoint p)
	{
		myPoint pt;
		pt.x = x - p.x;
		pt.y = y - p.y;
		return pt;
	}
	myPoint operator - (float fValue)
	{
		myPoint pt;
		pt.x = x - fValue;
		pt.y = y - fValue;
		return pt;
	}
	myPoint operator * (float fValue)
	{
		myPoint pt;
		pt.x = x * fValue;
		pt.y = y * fValue;
		return pt;
	}
	myPoint operator * (myPoint p)
	{
		myPoint pt;
		pt.x = x * p.x;
		pt.y = y * p.y;
		return pt;
	}
	float Length()
	{
		return sqrt(x * x + y * y);
	}
	myPoint Normalize()
	{
		float fLength = Length();
		if (fLength == 0.0f)
			fLength = EPSILON;
		x /= fLength;
		y /= fLength;
		return myPoint(x, y);
	}
	float GetAngle(myPoint& pA, myPoint& pB)
	{
		myPoint pt = pB - pA;
		float dist = pt.Length();
		float angle = acosf(pt.x / dist);

		if (pt.y > 0)
			angle = PI2M - angle;
		return angle;
	}
};