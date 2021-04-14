#pragma once
#include "myCore.h"
#include "myModelViewCamera.h"
#include "myHeightMap.h"
#include "myMiniMap.h"
#include "myQuadTree.h"
#include "myFbxLoader.h"
#include "myMouse.h"
#include "mySkyBox.h"

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
	Vector4			g_vLightDir[g_iNumLight]; // w = light damping(attenuation) : ����
	Vector4			g_vLightPos[g_iNumLight]; // w = light radius
	Vector4			g_vEyeDir[g_iNumLight];// w = light intensity : ����
	Vector4			g_vEyePos[g_iNumLight];// w = light radius		
};
struct LIGHT_CONSTANT_BUFFER3
{
	Vector4			g_vSpotInfo[g_iNumLight];
	//float			g_fFalloff;		// ���� �ܰ� �ܺ� ���� �ֵ�( Luminance )
	//float			g_fTheta;		// ���� ���� ���� ����		
	//float			g_fPhi;			// �ܺ� ���� ���� ����
};

class Sample : public myCore
{
public:
	myFbxObj* m_pFbxObj;
	myFbxObj* m_pFbxObj2;
	shared_ptr<myModelObject> m_pModelObj;
	mySkyBox* m_pSkyBox;
	myGameObject* m_pSkyBoxObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

//walk 3-35
//idle1 36-101
//idle2 102-161