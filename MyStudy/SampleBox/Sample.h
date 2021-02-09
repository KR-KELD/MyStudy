#pragma once
#include "myCore.h"
#include "myShape.h"

class Sample : public myCore
{
public:
	myShapeBox		m_Box;
	myShapePlane	m_Plane;
	myShapeLine		m_Line;
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
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

