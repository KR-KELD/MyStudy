#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	//fbx������Ʈ�� ������� gameobject �����͸� ä���ش�

	for (auto data : m_pModelObj->m_myNodeList)
	{
		myModelGraphics* pGraphics = data->GetComponent<myModelGraphics>();
		//����޽����� ���ؽ�����Ʈ,����,�ؽ��ĸ� ���� �������ش�
		for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
		{
			mySubMesh* pSub = &pGraphics->m_SubMeshList[iSub];
			if (pSub->m_TriangleList.size() <= 0) continue;
			pSub->m_VertexList.resize(pSub->m_TriangleList.size() * 3);
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

			// vbiw
			pSub->m_VertexListIW.resize(pSub->m_TriangleList.size() * 3);
			for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
			{
				int iIndex = iFace * 3;
				pSub->m_VertexListIW[iIndex + 0] =
					pSub->m_TriangleList[iFace].vVertexIW[0];
				pSub->m_VertexListIW[iIndex + 1] =
					pSub->m_TriangleList[iFace].vVertexIW[1];
				pSub->m_VertexListIW[iIndex + 2] =
					pSub->m_TriangleList[iFace].vVertexIW[2];
			}

			ID3D11Buffer* vbiw =
				CreateVertexBuffer(g_pd3dDevice,
					&pSub->m_VertexListIW.at(0),
					pSub->m_VertexListIW.size(),
					sizeof(IW_VERTEX));
			pSub->m_pVertexBufferIW.Attach(vbiw);

			wstring loadTex = pGraphics->m_MaterialList[iSub].c_str();
			pSub->m_pTexture = g_TextureMgr.Load(loadTex.c_str());
		}
		if (!pGraphics->Create(L"../../data/shader/vs.txt", L"../../data/shader/ps.txt", L""))
		{
			return false;
		}
	}

	return true;
}

bool Sample::Frame()
{
	myAnimation* pAnim = m_pModelObj->GetComponent<myAnimation>();
	pAnim->m_fTick += g_fSecondPerFrame *
		pAnim->m_AnimScene.iFrameSpeed *
		pAnim->m_AnimScene.iTickPerFrame;

	if (pAnim->m_fTick >=
		(pAnim->m_AnimScene.iLastFrame * pAnim->m_AnimScene.iTickPerFrame))
	{
		pAnim->m_fTick = 0;
	}

	for (int iNode = 0; iNode < m_pModelObj->m_myNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		myModelGraphics* pGraphics = 
			m_pModelObj->m_myNodeList[iNode]->GetComponent<myModelGraphics>();

		Matrix matBiped = Matrix::Identity;
		// ��Ų�� �����е�������� �̵���Ų��
		//std::string szName;
		//szName.assign(pModelObject->m_szName.begin(), pModelObject->m_szName.end());

		//auto data = m_pObj->m_dxMatrixBindPoseMap.find(szName);
		//if (data != m_pObj->m_dxMatrixBindPoseMap.end())
		//{
		//	matBiped = data->second;
		//}



		for (int iTick = 0; iTick < pGraphics->m_AnimTrackList.size(); iTick++)
		{
			if (pGraphics->m_AnimTrackList[iTick].iTick >= pAnim->m_fTick)
			{
				matWorld = pGraphics->m_AnimTrackList[iTick].matWorld;
				m_pModelObj->m_nodeMatList[iNode] = matBiped * matWorld;
				break;
			}
		}
	}

	Matrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_pModelObj->m_pBoneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (Matrix*)MappedFaceDest.pData;
		for (int dwObject = 0; dwObject < m_pModelObj->m_myNodeList.size(); dwObject++)
		{
			//��ġ (�����Ϸ���)
			Matrix matAnim = m_pModelObj->m_nodeMatList[dwObject].Transpose();
			pMatrices[dwObject] = matAnim;
		}
		g_pImmediateContext->Unmap(m_pModelObj->m_pBoneBuffer.Get(), 0);
	}

	return true;
}

bool Sample::Render()
{
	//���������δ� ��� �Լ��� ������ Render���� ���ư��Բ� �ؾ���
	//�װ� ȣ���ϴ°� obj�Ŵ����� �ִ� ����gameobject
	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pModelObj->m_pBoneBuffer.GetAddressOf());
	for (auto p: m_pModelObj->m_myNodeList)
	{
		myModelGraphics* pGraphics = p->GetComponent<myModelGraphics>();
		//p->m_iTick += g_fSecondPerFrame * p->m_p
		pGraphics->m_pTransform->SetMatrix(NULL,
			&g_pMainCamTransform->m_matView,
			&g_pMainCamTransform->m_matProj);
		pGraphics->m_cbData.vColor[0] = g_pMainCamTransform->m_vLook.x;
		pGraphics->m_cbData.vColor[1] = g_pMainCamTransform->m_vLook.y;
		pGraphics->m_cbData.vColor[2] = g_pMainCamTransform->m_vLook.z;
		pGraphics->Render();
	}
	
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
