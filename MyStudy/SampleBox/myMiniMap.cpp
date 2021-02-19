#include "myMiniMap.h"
DECLARE_COMPONENT(myMiniMap);
bool	myMiniMap::Create(ID3D11Device* pDevice,
	T_STR szVS, T_STR szPS,
	T_STR	szTex)
{
	m_DxRT.Set(pDevice);
	m_pd3dDevice = pDevice;
	m_DxRT.SetRenderTargetView();
	m_DxRT.SetDepthStencilView();
	m_DxRT.SetViewport();
	myGraphics::Create(pDevice, szVS, szPS, szTex);
	return true;
}
bool    myMiniMap::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(-0.5f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

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
bool myMiniMap::Begin(ID3D11DeviceContext*	pd3dContext)
{
	m_DxRT.Begin(pd3dContext);
	return true;
}
bool myMiniMap::End(ID3D11DeviceContext*	pd3dContext)
{
	m_DxRT.End(pd3dContext);
	return true;
}
bool myMiniMap::Render(ID3D11DeviceContext*	pd3dContext)
{
	myShapePlane::Render(pd3dContext);
	return true;
}

bool myMiniMap::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->PSSetShaderResources(0, 1, &m_DxRT.m_pSRV);
	myGraphics::PostRender(pd3dContext);
	m_DxRT.ClearShaderResources(pd3dContext);
	return true;
}