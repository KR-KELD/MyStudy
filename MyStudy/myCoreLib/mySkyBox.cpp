#include "mySkyBox.h"
DECLARE_COMPONENT(mySkyBox)

bool mySkyBox::Create(T_STR szVS, T_STR szPS, T_STR szTex)
{
	if (myGraphics::Create(szVS, szPS, szTex) == false)
	{
		MessageBox(0, _T("m_pDirectionLIne ����"), _T("Fatal error"), MB_OK);
		return 0;
	}
	wstring strPath = DataFolderPath;
	strPath += L"shader/";
	strPath += szPS;
	m_pPSTextureIndex.Attach(StaticGraphics::LoadPixelShaderFile(g_pd3dDevice, strPath.c_str(), "PS_TextureIndex"));
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

	vector<wstring> szSkyTex;
	wstring strTemp = L"";
	strTemp = DataFolderPath;
	strTemp += L"sky/st00_cm_front.bmp";
	szSkyTex.push_back(strTemp);

	strTemp = DataFolderPath;
	strTemp += L"sky/st00_cm_back.bmp";
	szSkyTex.push_back(strTemp);

	strTemp = DataFolderPath;
	strTemp += L"sky/st00_cm_left.bmp";
	szSkyTex.push_back(strTemp);

	strTemp = DataFolderPath;
	strTemp += L"sky/st00_cm_right.bmp";
	szSkyTex.push_back(strTemp);

	strTemp = DataFolderPath;
	strTemp += L"sky/st00_cm_up.bmp";
	szSkyTex.push_back(strTemp);

	strTemp = DataFolderPath;
	strTemp += L"sky/st00_cm_down.bmp";
	szSkyTex.push_back(strTemp);

	for (int iTex = 0; iTex < szSkyTex.size(); iTex++)
	{
		m_pTexSRV[iTex].Attach(StaticGraphics::CreateShaderResourceView(
			g_pd3dDevice,
			g_pImmediateContext, szSkyTex[iTex].c_str()));
	}
	strTemp = DataFolderPath;
	strTemp += L"sky/grassenvmap1024.dds";
	m_pTexCubeSRV.Attach(StaticGraphics::CreateShaderResourceView(
		g_pd3dDevice,
		g_pImmediateContext, strTemp.c_str()));
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
