#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	m_pFbxObj = g_FbxLoader.Load("ship.fbx");

	for (auto data : m_pFbxObj->m_MeshList)
	{
		myGraphics* pGraphics = data.second->GetComponent<myGraphics>();
		if (pGraphics->m_TriangleList.size() <= 0 &&
			pGraphics->m_SubMeshList.size() <= 0)
		{
			continue;
		}
		if (pGraphics->m_SubMeshList.size() == 0)
		{
			pGraphics->m_VertexList.resize(pGraphics->m_TriangleList.size() * 3);
			for (int iFace = 0; iFace < pGraphics->m_TriangleList.size(); iFace++)
			{
				int iIndex = iFace * 3;
				pGraphics->m_VertexList[iIndex + 0] =
					pGraphics->m_TriangleList[iFace].vVertex[0];
				pGraphics->m_VertexList[iIndex + 1] =
					pGraphics->m_TriangleList[iFace].vVertex[1];
				pGraphics->m_VertexList[iIndex + 2] =
					pGraphics->m_TriangleList[iFace].vVertex[2];
			}

			T_STR loadTexName = pGraphics->m_MaterialList[0];
			if (!pGraphics->Create(L"../../data/shader/vs.txt", L"../../data/shader/ps.txt", loadTexName))
			{
				return false;
			}
		}
		else
		{
			for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
			{
				mySubMesh* pSub = &pGraphics->m_SubMeshList[iSub];
				pSub->m_VertexList.resize(
					pSub->m_TriangleList.size() * 3);
				for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pSub->m_VertexList[iIndex + 0] =
						pSub->m_TriangleList[iFace].vVertex[0];
					pSub->m_VertexList[iIndex + 1] =
						pSub->m_TriangleList[iFace].vVertex[1];
					pSub->m_VertexList[iIndex + 2] =
						pSub->m_TriangleList[iFace].vVertex[2];
				}

				// vb
				ID3D11Buffer* vb =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_VertexList.at(0),
						pSub->m_VertexList.size(),
						sizeof(PNCT_VERTEX));
				pSub->m_pVertexBuffer.Attach(vb);
				wstring loadTex = pGraphics->m_MaterialList[iSub].c_str();
				pSub->m_pTexture = g_TextureMgr.Load(loadTex.c_str());
			}
			if (!pGraphics->Create(L"../../data/shader/vs.txt", L"../../data/shader/ps.txt", L""))
			{
				return false;
			}
		}
	}
	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject

	for (auto data : m_pFbxObj->m_MeshList)
	{
		myGraphics* pGraphics = data.second->GetComponent<myGraphics>();
		if (pGraphics->m_SubMeshList.size() == 0)
		{
			if (pGraphics->m_TriangleList.size() <= 0) continue;

			pGraphics->m_pTransform->SetMatrix(&pGraphics->m_pTransform->m_matWorld,
				&g_pMainCamTransform->m_matView,
				&g_pMainCamTransform->m_matProj);
			pGraphics->Render();
		}
		else
		{
			pGraphics->m_pTransform->SetMatrix(&pGraphics->m_pTransform->m_matWorld,
				&g_pMainCamTransform->m_matView,
				&g_pMainCamTransform->m_matProj);
			pGraphics->Update();
			pGraphics->PreRender();
			UINT iStride = sizeof(PNCT_VERTEX);
			UINT iOffset = 0;
			g_pImmediateContext->IASetIndexBuffer(pGraphics->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			g_pImmediateContext->IASetInputLayout(pGraphics->m_pInputLayout.Get());
			g_pImmediateContext->VSSetConstantBuffers(0, 1, pGraphics->m_pConstantBuffer.GetAddressOf());
			g_pImmediateContext->PSSetConstantBuffers(0, 1, pGraphics->m_pConstantBuffer.GetAddressOf());
			g_pImmediateContext->VSSetShader(pGraphics->m_pVertexShader.Get(), NULL, 0);
			g_pImmediateContext->PSSetShader(pGraphics->m_pPixelShader.Get(), NULL, 0);
			g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)pGraphics->m_iTopology);

			for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
			{
				mySubMesh* pMesh = &pGraphics->m_SubMeshList[iSub];
				if (pMesh->m_TriangleList.size() <= 0) continue;
				g_pImmediateContext->IASetVertexBuffers(0, 1,
					pMesh->m_pVertexBuffer.GetAddressOf(), &iStride, &iOffset);
				if (pMesh->m_pTexture != nullptr)
				{
					g_pImmediateContext->PSSetShaderResources(0, 1,
						pMesh->m_pTexture->m_pTextureSRV.GetAddressOf());
				}
				if (pGraphics->m_pIndexBuffer.Get() == nullptr)
				{
					g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
				}
				else
				{
					PostRender();
				}
			}
		}
	}
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
