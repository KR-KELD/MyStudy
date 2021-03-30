#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");

	//fbx오브젝트를 기반으로 gameobject 데이터를 채워준다

	for (auto data : m_pFbxObj->m_NodeList)
	{
		myModelGraphics* pGraphics = data.second->GetComponent<myModelGraphics>();
		//그래픽 정보가 없으면 넘어가고
		if (pGraphics->m_TriangleList.size() <= 0 &&
			pGraphics->m_SubMeshList.size() <= 0)
		{
			continue;
		}
		//서브메쉬가 없을때(메테리얼을 1개만 쓰는 오브젝트)
		if (pGraphics->m_SubMeshList.size() == 0)
		{
			//정점 정보와 텍스쳐를 불러온다
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
		//서브메쉬가 있을때(한 오브젝트에 여러개의 서브메테리얼을 사용)
		else
		{
			//서브메쉬마다 버텍스리스트,버퍼,텍스쳐를 각각 세팅해준다
			for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
			{
				mySubMesh* pSub = &pGraphics->m_SubMeshList[iSub];
				if (pSub->m_TriangleList.size() <= 0) continue;
				pSub->m_VertexList.resize( pSub->m_TriangleList.size() * 3);
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
	}
	m_pModelObj = m_pFbxObj->m_pModelObject;
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
		for (int iTick = 0; iTick < pGraphics->m_AnimTrackList.size(); iTick++)
		{
			if (pGraphics->m_AnimTrackList[iTick].iTick >=
				pAnim->m_fTick)
			{
				matWorld = pGraphics->m_AnimTrackList[iTick].matWorld;
				m_pModelObj->m_nodeMatList[iNode] = matWorld;
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
			//전치 (내적하려고)
			Matrix matAnim = m_pModelObj->m_nodeMatList[dwObject].Transpose();
			pMatrices[dwObject] = matAnim;
		}
		g_pImmediateContext->Unmap(m_pModelObj->m_pBoneBuffer.Get(), 0);
	}

	return true;
}

bool Sample::Render()
{
	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject

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
