#pragma once
#include "myCore.h"
#include "myShape.h"
#include "myFigure.h"

class Sample : public myCore
{
public:
	myShapeBox		m_Box;
	myShapePlane	m_Plane;
	myShapeLine		m_Line;
public:
	//myBox			m_Box[3];
public:
	myMatrix			m_matBoxWorld;
	myMatrix			m_matPlaneWorld;
public:
	float			m_CameraAngleX = 0.0f;
	float			m_CameraAngleY = 0.0f;
	myMatrix		m_matWorld;
	myMatrix		m_matView;
	myMatrix		m_matProj;
	myVector3		m_vCameraPos;
	myVector3		m_vCameraTarget;
public:
	ID3D11RasterizerState*		m_pRSSolidBack;
	ID3D11RasterizerState*		m_pRSWireBack;
	ID3D11RasterizerState*		m_pRS;
	ID3D11SamplerState*			m_pWrapLinear;
	D3D11_FILL_MODE				m_FillMode;
	D3D11_CULL_MODE				m_CullMode;
	//ID3D11RasterizerState*	m_pRS;
public:
	ID3D11DepthStencilView*		m_pDSV;
	ID3D11DepthStencilState*	m_pDSS;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool PreRender() override;

public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
	void SetRasterizerState();
};

