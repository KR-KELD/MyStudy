#pragma once
#include "myGameObject.h"

const int g_iNumLight = 3;
struct LIGHT_CONSTANT_BUFFER1
{
	// Material
	Vector4			g_cAmbientMaterial[g_iNumLight];
	Vector4			g_cDiffuseMaterial[g_iNumLight];
	Vector4			g_cSpecularMaterial[g_iNumLight];
	Vector4			g_cEmissionMaterial[g_iNumLight];
	// Light
	Vector4			g_cAmbientLightColor[g_iNumLight];
	Vector4			g_cDiffuseLightColor[g_iNumLight];
	Vector4			g_cSpecularLightColor[g_iNumLight];
};
struct LIGHT_CONSTANT_BUFFER2
{
	Matrix			g_matInvWorld[g_iNumLight];
	Vector4			g_vLightDir[g_iNumLight]; // w = light damping(attenuation) : 감쇠
	Vector4			g_vLightPos[g_iNumLight]; // w = light radius
	Vector4			g_vEyeDir[g_iNumLight];// w = light intensity : 강도
	Vector4			g_vEyePos[g_iNumLight];// w = light radius		
};
struct LIGHT_CONSTANT_BUFFER3
{
	Vector4			g_vSpotInfo[g_iNumLight];
	//float			g_fFalloff;		// 내부 콘과 외부 콘의 휘도( Luminance )
	//float			g_fTheta;		// 내부 콘의 각도 범위		
	//float			g_fPhi;			// 외부 콘의 각도 범위
};

class myLight : public myComponent
{
public:
	DEFINE_COMPONENT(myLight, myComponent, false)
public:

public:
	myLight();
	virtual ~myLight();
};

