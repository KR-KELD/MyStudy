#include "myVector.h"


myVector3::myVector3()
{
	x = y = z = 0.0f;
}

myVector3::myVector3(const myVector3& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
}

myVector3::myVector3(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

// 연산자 재정의 
myVector3 myVector3::operator + (myVector3 const &v0)
{
	return myVector3(v0.x + x, v0.y + y, v0.z + z);
}

myVector3 myVector3::operator - (myVector3 const &v0)
{
	return myVector3(x - v0.x, y - v0.y, z - v0.z);
}

myVector3 myVector3::operator + (float const &fScala)
{
	return myVector3(fScala + x, fScala + y, fScala + z);
}
myVector3 myVector3::operator * (float const &fScala)
{
	return myVector3(fScala*x, fScala*y, fScala*z);
}
myVector3 myVector3::operator / (float const &fScala)
{
	float fInvert = 1.0f / fScala;
	return myVector3(x*fInvert, y*fInvert, z*fInvert);
}

// Dot Product
float myVector3::operator | (myVector3 const &v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// Cross Product
myVector3 myVector3::operator ^ (myVector3 const &v0)
{
	return myVector3((y*v0.z - z * v0.y), (z*v0.x - x * v0.z), (x*v0.y - y * v0.x));
}

bool	myVector3::operator == (myVector3 const &v0)
{
	if (fabs(x - v0.x) < EPSILON)
	{
		if (fabs(y - v0.y) < EPSILON)
		{
			if (fabs(z - v0.z) < EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}

// 제곱
float myVector3::LengthSquared()
{
	return (x*x + y * y + z * z);
}

// 원점으로 부터의 거리
float myVector3::Length()
{
	return (float)sqrt(LengthSquared());
}

myVector3 myVector3::Normal()
{
	float invertLength = 1.0f / Length();
	return myVector3(x*invertLength, y*invertLength, z*invertLength);
}

float myVector3::Angle(myVector3& v0)
{
	float fLength1 = Length();
	float fLength2 = v0.Length();
	float fCosAngle = ((*this) | v0) / (fLength1*fLength2);
	return (float)RA_TO_DE(acos(fCosAngle));
}


myVector4::myVector4()
{
	x = y = z = w = 0.0f;
}

myVector4::myVector4(const myVector4& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
	w = v0.w;
}

myVector4::myVector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}

bool myVector4::operator==(myVector4 const & v0)
{
	if (fabs(x - v0.x) < EPSILON)
	{
		if (fabs(y - v0.y) < EPSILON)
		{
			if (fabs(z - v0.z) < EPSILON)
			{
				if (fabs(w - v0.w) < EPSILON)
				{
					return true;
				}
			}
		}
	}
	return false;
}

float myVector4::operator|(myVector4 const & v0)
{
	return x * v0.x + y * v0.y + z * v0.z + w * v0.w;
}

float myVector4::Length()
{
	return (float)sqrt((x*x + y * y + z * z + w * w));
}

myVector4 myVector4::Normal()
{
	float invertLength = 1.0f / Length();
	return myVector4(x*invertLength, y*invertLength, z*invertLength, w*invertLength);
}


myVector2::myVector2()
{
	x = y = 0.0f;
}

myVector2::myVector2(const myVector2& v0)
{
	x = v0.x;
	y = v0.y;
}

myVector2::myVector2(float fX, float fY)
{
	x = fX;
	y = fY;
}

bool myVector2::operator==(myVector2 const & v0)
{
	if (fabs(x - v0.x) < EPSILON)
	{
		if (fabs(y - v0.y) < EPSILON)
		{
			return true;
		}
	}
	return false;
}
