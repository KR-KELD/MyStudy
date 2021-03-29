#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");

	//fbx������Ʈ�� ������� gameobject �����͸� ä���ش�

	for (auto data : m_pFbxObj->m_MeshList)
	{
		myModelGraphics* pGraphics = data.second->GetComponent<myModelGraphics>();
		//�׷��� ������ ������ �Ѿ��
		if (pGraphics->m_TriangleList.size() <= 0 &&
			pGraphics->m_SubMeshList.size() <= 0)
		{
			continue;
		}
		//����޽��� ������(���׸����� 1���� ���� ������Ʈ)
		if (pGraphics->m_SubMeshList.size() == 0)
		{
			//���� ������ �ؽ��ĸ� �ҷ��´�
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

			T_STR loadTexName;
			if (pGraphics->m_MaterialList.size())
			{
				loadTexName = pGraphics->m_MaterialList[0];
			}
			if (!pGraphics->Create(L"../../data/shader/vs.txt", L"../../data/shader/ps.txt", loadTexName))
			{
				return false;
			}
		}
		//����޽��� ������(�� ������Ʈ�� �������� ������׸����� ���)
		else
		{
			//����޽����� ���ؽ�����Ʈ,����,�ؽ��ĸ� ���� �������ش�
			for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
			{
				mySubMesh* pSub = &pGraphics->m_SubMeshList[iSub];
				if (pSub->m_TriangleList.size() <= 0) continue;
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
		m_graphicObj.push_back(pGraphics);
	}

	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	//���������δ� ��� �Լ��� ������ Render���� ���ư��Բ� �ؾ���
	//�װ� ȣ���ϴ°� obj�Ŵ����� �ִ� ����gameobject

	for (auto p: m_graphicObj)
	{
		//p->m_iTick += g_fSecondPerFrame * p->m_p
		p->m_pTransform->SetMatrix(&p->m_pTransform->m_matWorld,
			&g_pMainCamTransform->m_matView,
			&g_pMainCamTransform->m_matProj);
		p->m_cbData.vColor[0] = g_pMainCamTransform->m_vLook.x;
		p->m_cbData.vColor[1] = g_pMainCamTransform->m_vLook.y;
		p->m_cbData.vColor[2] = g_pMainCamTransform->m_vLook.z;
		p->Render();
	}
	
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
