#pragma once
#include "myGraphics.h"

class myShape : public myGraphics
{
public:
	DEFINE_COMPONENT(myShape, null_t, false);
public:
	Vector3			m_vCenter;
	float			m_fRange;
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateVertexData(Vector3& vCenter, float& fRange)override;
	virtual bool    CreateIndexData()override;
public:
	myShape();
	virtual ~myShape();
};

class myShapeBox : public myShape
{
public:
	DEFINE_COMPONENT(myShapeBox, null_t, false);
public:
	virtual bool    CreateVertexData(Vector3& vCenter, float& fRange)override;
	virtual bool    CreateIndexData()override;
public:
	myShapeBox();
	virtual ~myShapeBox();
};

class myShapePlane : public myShape
{
public:
	DEFINE_COMPONENT(myShapePlane, null_t, false);
public:
	virtual bool    CreateVertexData(Vector3& vCenter, float& fRange)override;
	virtual bool    CreateIndexData()override;
public:
	myShapePlane();
	virtual ~myShapePlane();
};

class myShapeLine : public myShape
{
public:
	DEFINE_COMPONENT(myShapeLine, null_t, false);
public:
	bool Draw(ID3D11DeviceContext* pd3dContext,
		Vector3 p, Vector3 e, Vector4 c = Vector4(1, 0, 0, 1));
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	myShapeLine();
	virtual ~myShapeLine();
};
