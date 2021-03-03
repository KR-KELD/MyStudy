#pragma once
#include "myCore.h"
#include "myShape.h"
#include "myModelViewCamera.h"
#include "myHeightMap.h"
#include "myMiniMap.h"
#include "myQuadTree.h"

class Sample : public myCore
{
public:
	myQuadTree		m_QuadTree;

	myHeightMap*	m_Map;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	Matrix CreateMatrixShadow(Vector4* pPlane, Vector4* pLight);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool DrawQuadLine(myNode* pNode);
	//void DrawObject(Matrix* pView, Matrix* pProj);
};
