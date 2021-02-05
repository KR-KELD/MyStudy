#pragma once
#include "myStd.h"

#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)




struct myVector3 : public float3
{
public:
	myVector3();
	myVector3(const myVector3& v0);
	myVector3(float fX, float fY, float fZ);
	// 연산자 재정의 
	myVector3 operator + (myVector3 const &v0);
	myVector3 operator - (myVector3 const &v0);
	myVector3 operator * (float const &fScala);
	myVector3 operator + (float const &fScala);
	myVector3 operator / (float const &fScala);
	// Dot Product
	float operator | (myVector3 const &v0);
	// Cross Product
	myVector3 operator ^ (myVector3 const &v0);
	bool	operator == (myVector3 const &v0);
	// 제곱
	float LengthSquared();
	// 원점으로 부터의 거리
	float Length();
	myVector3 Normal();
	float Angle(myVector3& v0);
};

struct myVector4 : public float4
{
public:
	myVector4();
	myVector4(const myVector4& v0);
	myVector4(float fX, float fY, float fZ, float fW);
};

struct myVector2 : public float2
{
public:
	myVector2();
	myVector2(const myVector2& v0);
	myVector2(float fX, float fY);
};