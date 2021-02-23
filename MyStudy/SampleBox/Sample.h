#pragma once
#include "myCore.h"
#include "myShape.h"
#include "myModelViewCamera.h"
#include "myMap.h"
#include "myMiniMap.h"

class Sample : public myCore
{
public:
	myMiniMap*		m_MiniMap;
	myCamera*		m_TopCamera;
	myMap*			m_Map;
	Vector4			m_vDirValue;
	myShapeBox*		m_Box;
	myShapePlane*	m_Plane;
	myShapeLine*	m_Line;
public:
	Matrix				m_matBoxWorld;
	Matrix				m_matPlaneWorld;
	myModelViewCamera*	m_ModelCamera;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	Matrix CreateMatrixShadow(Vector4* pPlane, Vector4* pLight);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

