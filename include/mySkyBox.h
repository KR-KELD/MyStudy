#pragma once
#include "myShape.h"
#include "myDxState.h"
#define MAX_SKYBOX_TEXTURE	6
class mySkyBox : public myShapeBox
{
public:
	DEFINE_COMPONENT(mySkyBox, myShapeBox, false);
public:
	ComPtr<ID3D11ShaderResourceView>	m_pTexSRV[MAX_SKYBOX_TEXTURE];
	ComPtr<ID3D11ShaderResourceView>	m_pTexCubeSRV;
	ComPtr<ID3D11PixelShader>			m_pPSTextureIndex;
	bool								m_bRenderType;
public:
	bool				Create(T_STR szVS, T_STR szPS, T_STR	szTex);
	bool				Render(ID3D11DeviceContext*	pd3dContext);
	bool				LoadTexture(T_STR szTex);
	bool				Draw(ID3D11DeviceContext*	pd3dContext);
public:
	mySkyBox();
	~mySkyBox();
};

