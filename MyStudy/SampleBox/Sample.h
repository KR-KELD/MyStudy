#pragma once
#include "myCore.h"
#include "myShape.h"
#include "myModelViewCamera.h"
#include "myHeightMap.h"
#include "myMiniMap.h"

class Sample : public myCore
{
public:
	myMiniMap*		m_MiniMap;
	myCamera*		m_TopCamera;
	myHeightMap*	m_Map;
	myShapeBox*		m_Box;
public:
	Matrix				m_matBoxWorld;
	Matrix				m_matPlaneWorld;
	myGameObject*	m_ModelCameraObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	Matrix CreateMatrixShadow(Vector4* pPlane, Vector4* pLight);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

