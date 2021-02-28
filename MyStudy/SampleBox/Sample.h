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
	MY_BoxObject*	m_pObject;
	MY_BOX			m_myBoxBase;


	myMiniMap*		m_MiniMap;
	myCamera*		m_TopCamera;
	myHeightMap*	m_Map;
	myShapeBox*		m_Box;
	myGameObject*	m_MiniObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	Matrix CreateMatrixShadow(Vector4* pPlane, Vector4* pLight);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool DrawQuadLine(myNode* pNode);
	void DrawObject(Matrix* pView, Matrix* pProj);
};

