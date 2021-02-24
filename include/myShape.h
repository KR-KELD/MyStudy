#pragma once
#include "myGraphics.h"

class myShape : public myGraphics
{
public:
	DEFINE_COMPONENT(myShape, myGraphics, false);
public:
	float			m_fRange;
public:
	virtual bool	Init();
	virtual bool    CreateVertexData()override;
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange)override;
	virtual bool    CreateIndexData()override;
public:
	myShape();
	virtual ~myShape();
};

class myShapeBox : public myShape
{
public:
	DEFINE_COMPONENT(myShapeBox, myShape, false);
public:
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange)override;
	virtual bool    CreateIndexData()override;
public:
	myShapeBox();
	virtual ~myShapeBox();
};

class myShapePlane : public myShape
{
public:
	DEFINE_COMPONENT(myShapePlane, myShape, false);
public:
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange)override;
	virtual bool    CreateIndexData()override;
public:
	myShapePlane();
	virtual ~myShapePlane();
};

class myShapeLine : public myShape
{
public:
	DEFINE_COMPONENT(myShapeLine, myShape, false);
public:
	bool Draw(Vector3 p, Vector3 e, Vector4 c = Vector4(1, 0, 0, 1));
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	myShapeLine();
	virtual ~myShapeLine();
};
