#include "myMiniMap.h"
DECLARE_COMPONENT(myMiniMap);
bool	myMiniMap::Create(T_STR szVS, T_STR szPS, T_STR	szTex)
{
	m_DxRT.SetRenderTargetView();
	m_DxRT.SetDepthStencilView();
	m_DxRT.SetViewport();
	myGraphics::Create(szVS, szPS, szTex);
	return true;
}
bool    myMiniMap::CreateVertexData()
{
	myShapePlane::CreateVertexData(Vector3(-0.75f,0.75f,0.0f),0.25f);
	m_pTransform->m_vPos = Vector3(0.0f, 0.0f, 0.0f);
	//m_VertexList.resize(4);
	//m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	//m_VertexList[1] = PNCT_VERTEX(Vector3(-0.5f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	//m_VertexList[2] = PNCT_VERTEX(Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	//m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	return true;
}
bool    myMiniMap::CreateIndexData()
{
	myShapePlane::CreateIndexData();
	return true;
}
bool  myMiniMap::Release()
{
	m_DxRT.Release();
	return myGraphics::Release();
}

bool myMiniMap::Begin()
{
	m_DxRT.Begin();
	return true;
}
bool myMiniMap::End()
{
	m_DxRT.End();
	return true;
}

bool myMiniMap::Draw()
{
	g_pImmediateContext->PSSetShaderResources(0, 1, m_DxRT.m_pSRV.GetAddressOf());
	myGraphics::PostRender();
	m_DxRT.ClearShaderResources();
	return true;
}
