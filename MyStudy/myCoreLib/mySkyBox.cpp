#include "mySkyBox.h"
DECLARE_COMPONENT(mySkyBox);

bool mySkyBox::Create(T_STR szVS, T_STR szPS, T_STR szTex)
{
	if (myGraphics::Create(szVS, szPS, szTex) == false)
	{
		MessageBox(0, _T("m_pDirectionLIne ����"), _T("Fatal error"), MB_OK);
		return 0;
	}
	m_pPSTextureIndex.Attach(LoadPixelShaderFile(g_pd3dDevice, szPS.c_str(), "PS_TextureIndex"));
	return true;
}

bool mySkyBox::Render(ID3D11DeviceContext*	pd3dContext)
{
	m_pTransform->m_matWorld = Matrix::CreateScale(100.0f, 100.0f, 100.0f);
	Matrix matSkyBoxView = m_pTransform->m_matView;
	matSkyBoxView._41 = 0;
	matSkyBoxView._42 = 0;
	matSkyBoxView._43 = 0;

	g_pImmediateContext->RSSetState(myDxState::g_pRSNoneCullSolid);
	g_pImmediateContext->PSSetSamplers(0, 1, &myDxState::g_pSSClampPoint);
	g_pImmediateContext->OMSetDepthStencilState(myDxState::g_pDSSDepthDisable, 0);
	/*TBASIS_CORE_LIB::ApplyDSS(pContext,
		TBASIS_CORE_LIB::TDxState::g_pDSSDepthDisable);

	ID3D11SamplerState* ppSamplerStates[2] = {
		TDxState::g_pSSClampPoint,
		TDxState::g_pSSWrapLinear };
	pContext->PSSetSamplers(0, 2, ppSamplerStates);*/

	m_pTransform->SetMatrix(&m_pTransform->m_matWorld, &matSkyBoxView, &m_pTransform->m_matProj);
	myGraphics::Render(pd3dContext);
	return true;
}

bool mySkyBox::LoadTexture(T_STR szTex)
{
	HRESULT hr = S_OK;
	//�ؽ��Ĺ����� �ٸ���
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
	m_pTexCubeSRV.Attach(CreateShaderResourceView(
		g_pd3dDevice,
		g_pImmediateContext, L"..\\..\\data\\sky\\grassenvmap1024.dds"));
	return true;
}

bool mySkyBox::Draw(ID3D11DeviceContext*	pd3dContext)
{
	//ť���ؽ��� ���� 1�����Կ�
	pd3dContext->PSSetShaderResources(1, 1, m_pTexCubeSRV.GetAddressOf());
	if (m_bRenderType)
	{
		//--------------------------------------------------------------------------------------
		// ���徿 ������ �ϴ� ���
		//--------------------------------------------------------------------------------------
		for (int iTex = 0; iTex < MAX_SKYBOX_TEXTURE; iTex++)
		{
			if (m_pTexSRV[iTex] == nullptr) break;
			pd3dContext->PSSetShaderResources(0, 1, m_pTexSRV[iTex].GetAddressOf());
			pd3dContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
			// �������� ����� �ε��� ���� ����, �ε��� ���� ����, ���ؽ� ���� ����
			pd3dContext->DrawIndexed(6, 6 * iTex, 0);
		}
	}
	else
	{
		//��ò���
		//pd3dContext->PSSetShaderResources(2, 6, m_pTexSRV[0].GetAddressOf());
		//pd3dContext->PSSetShader(m_pPSTextureIndex.Get(), NULL, 0);
		pd3dContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
		// �������� ����� �ε��� ���� ����, �ε��� ���� ����, ���ؽ� ���� ����
		pd3dContext->DrawIndexed(36, 0, 0);
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
