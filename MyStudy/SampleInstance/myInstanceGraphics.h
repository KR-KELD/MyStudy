#pragma once
#include "myShape.h"
class myInstanceGraphics :
	public myGraphics
{
public:
	DEFINE_COMPONENT(myInstanceGraphics, myGraphics, true);
public:
	const int				m_iNumIntance = 100;
	float					m_fRange;
	ComPtr<ID3D11Buffer>	m_pInstanceBuffer;
	vector<myBaseInstance>	m_InstanceList;
public:
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange);
	virtual bool    CreateVertexData();
	virtual bool    CreateIndexData();

	virtual bool	CreateInputLayout()override;
	virtual bool	CreateInstanceBuffer();
	virtual bool	InstancingRender(ID3D11DeviceContext*	pd3dContext, int i);
public:
	myInstanceGraphics();
	virtual ~myInstanceGraphics();
};

