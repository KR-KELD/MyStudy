#include "mySkyBox.h"
DECLARE_COMPONENT(mySkyBox);

bool mySkyBox::Create(T_STR szVS, T_STR szPS, T_STR szTex)
{
	if (myGraphics::Create(szVS, szPS, szTex) == false)
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pPSTextrueIndex.Attach(LoadPixelShaderFile(g_pd3dDevice, szPS.c_str(), "PS_TextureIndex"));
	return true;
}

bool mySkyBox::Render()
{
	m_pTransform->m_matWorld = Matrix::CreateScale(100.0f, 100.0f, 100.0f);
	Matrix matSkyBoxView = m_pTransform->m_matView;
	matSkyBoxView._41 = 0;
	matSkyBoxView._42 = 0;
	matSkyBoxView._43 = 0;

	g_pImmediateContext->RSSetState(myDxState::m_pRSSolidNone.Get());
	g_pImmediateContext->PSSetSamplers(0, 1, myDxState::m_pWrapLinear.GetAddressOf());
	g_pImmediateContext->OMSetDepthStencilState(myDxState::m_pDSS.Get(), 0);
	/*TBASIS_CORE_LIB::ApplyDSS(pContext,
		TBASIS_CORE_LIB::TDxState::g_pDSSDepthDisable);

	ID3D11SamplerState* ppSamplerStates[2] = {
		TDxState::g_pSSClampPoint,
		TDxState::g_pSSWrapLinear };
	pContext->PSSetSamplers(0, 2, ppSamplerStates);*/

	m_pTransform->SetMatrix(&m_pTransform->m_matWorld, &matSkyBoxView, &m_pTransform->m_matProj);
	myGraphics::Render();
	return true;
}

bool mySkyBox::LoadTexture(T_STR szTex)
{
	HRESULT hr = S_OK;
	//텍스쳐방향이 다르다
	const TCHAR* g_szSkyTextures[] =
	{
		L"..\\..\\data\\sky\\st00_cm_front.bmp",
		L"..\\..\\data\\sky\\st00_cm_back.bmp",
		L"..\\..\\data\\sky\\st00_cm_left.bmp",
		L"..\\..\\data\\sky\\st00_cm_right.bmp",
		L"..\\..\\data\\sky\\st00_cm_up.bmp",
		L"..\\..\\data\\sky\\st00_cm_down.bmp"
	};
	int iNumTexture = sizeof(g_szSkyTextures) / sizeof(g_szSkyTextures[0]);

	for (int iTex = 0; iTex < iNumTexture; iTex++)
	{
		m_pTexSRV[iTex].Attach(CreateShaderResourceView(
			g_pd3dDevice,
			g_pImmediateContext, g_szSkyTextures[iTex]));
	}

	return true;
}

bool mySkyBox::Draw()
{
	if (m_bRenderType)
	{
		//--------------------------------------------------------------------------------------
		// 한장씩 랜더링 하는 방법
		//--------------------------------------------------------------------------------------
		for (int iTex = 0; iTex < MAX_SKYBOX_TEXTURE; iTex++)
		{
			if (m_pTexSRV[iTex] == nullptr) break;
			g_pImmediateContext->PSSetShaderResources(0, 1, m_pTexSRV[iTex].GetAddressOf());
			g_pImmediateContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
			// 랜더링에 사용할 인덱스 버퍼 갯수, 인덱스 버퍼 시작, 버텍스 버퍼 시작
			g_pImmediateContext->DrawIndexed(6, 6 * iTex, 0);
		}
	}
	else
	{
		g_pImmediateContext->PSSetShaderResources(1, 6, m_pTexSRV[0].GetAddressOf());
		g_pImmediateContext->PSSetShader(m_pPSTextrueIndex.Get(), NULL, 0);
		// 랜더링에 사용할 인덱스 버퍼 갯수, 인덱스 버퍼 시작, 버텍스 버퍼 시작
		g_pImmediateContext->DrawIndexed(36, 0, 0);
	}
	return true;
}

mySkyBox::mySkyBox()
{
	m_bRenderType = false;
}

mySkyBox::~mySkyBox()
{
}
