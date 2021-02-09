#pragma once
#include "myStd.h"


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
	bool	operator == (myVector4 const &v0);
	float operator | (myVector4 const &v0);
	float Length();
	myVector4 Normal();
};

struct myVector2 : public float2
{
public:
	myVector2();
	myVector2(const myVector2& v0);
	myVector2(float fX, float fY);
	bool	operator == (myVector2 const &v0);
};