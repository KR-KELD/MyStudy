#pragma once
#include "myCore.h"
#include "myShape.h"
#include "myCamera.h"

class Sample : public myCore
{
public:
	myVector4		m_vDirValue;
	myShapeBox		m_Box;
	myShapePlane	m_Plane;
	myShapeLine		m_Line;
public:
	myMatrix		m_matBoxWorld;
	myMatrix		m_matPlaneWorld;
	myCamera		m_Camera;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	myMatrix CreateMatrixShadow(myVector4* pPlane, myVector4* pLight);
};

