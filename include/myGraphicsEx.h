#pragma once
#include "myModelGraphics.h"
//보류
//트라이앵글리스트도 다 바꿔야하나


class myGraphicsEx : public myModelGraphics
{
public:
	DEFINE_COMPONENT(myGraphicsEx, myModelGraphics, true);
public:
	vector<PNCT2_VERTEX>	m_VertexList2;
public:
	virtual bool    CreateVertexBuffer() override;
	virtual bool	CreateInputLayout() override;
public:

	virtual bool	MultiDraw(ID3D11DeviceContext*	pd3dContext) override;
	//virtual void SetTangent();
public:
	myGraphicsEx();
	virtual ~myGraphicsEx();
};

