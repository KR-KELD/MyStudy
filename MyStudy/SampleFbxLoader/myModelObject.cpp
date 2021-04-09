#include "myModelObject.h"
#include "myCameraManager.h"
#define MAX_BONE_MATRICES 255
DECLARE_COMPONENT(myModelObject);
//bool myModelObject::SetAnimScene(wstring strSceneName, myAnimScene & anim)
//{
//	m_pAnim->m_AnimSceneMap.insert(make_pair(strSceneName, myAnimScene(anim)));
//	return true;
//}
int myModelObject::SetAnimTrack(vector<myGameObject*>& nodeList)
{
	if (nodeList.size() != m_myNodeList.size()) return -1;
	int iTrackIndex = -1;
	for (int iNode = 0; iNode < m_myNodeList.size(); iNode++)
	{
		myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();
		myModelGraphics* pSourceGraphics = nodeList[iNode]->GetComponent<myModelGraphics>();
		if (pGraphics != nullptr && pSourceGraphics != nullptr)
		{
			if (pSourceGraphics->m_AnimTrackList.size() <= 0) continue;
			pGraphics->m_AnimTrackList.emplace_back(pSourceGraphics->m_AnimTrackList.front());
			if (iTrackIndex < 0)
			{
				iTrackIndex = pGraphics->m_AnimTrackList.size() - 1;
			}
		}
	}
	return iTrackIndex;
}
bool myModelObject::SetAnim(wstring strSceneName, myAnimScene & scene, vector<myGameObject*>& nodeList)
{
	int iTrackIndex = SetAnimTrack(nodeList);
	if (iTrackIndex == -1) return false;
	scene.ptAnimTrackIndex.x = iTrackIndex;
	m_pAnim->AddAnim(strSceneName, scene);
	return true;
}
bool myModelObject::Frame()
{
	myGameObject::PreFrame();
	myGameObject::Frame();
	myAnimScene* pScene = m_pAnim->m_pCurrentScene;
	if (pScene != nullptr)
	{
		m_pAnim->m_fTick += g_fSecondPerFrame *
			pScene->iFrameSpeed *
			pScene->iTickPerFrame;

		if (m_pAnim->m_fTick >=
			(pScene->iLastFrame * pScene->iTickPerFrame))
		{
			m_pAnim->m_fTick = pScene->iFirstFrame *
				pScene->iTickPerFrame;
		}

		for (int iNode = 0; iNode < m_myNodeList.size(); iNode++)
		{
			Matrix matWorld = Matrix::Identity;
			Matrix matBiped = Matrix::Identity;
			Matrix matParent = Matrix::Identity;
			myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();

			// 스킨을 바이패드공간으로 이동시킨다
			string szName = to_wm(pGraphics->m_pGameObject->m_strName);


			auto data = m_nodeMatBindPoseMap.find(szName);
			if (data != m_nodeMatBindPoseMap.end())
			{
				matBiped = data->second;
			}

			if (m_myNodeList[iNode]->m_pParent != nullptr)
			{
				matParent = m_myNodeList[iNode]->m_pParent->m_pTransform->m_matAnim;
			}

			for (int iTick = 1; iTick < pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x].size(); iTick++)
			{
				if (pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick].iTick >=
					m_pAnim->m_fTick)
				{
					int iStart = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick - 1].iTick;
					int iEnd = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick].iTick;
					int iStepTick = iEnd - iStart;
					float t = (m_pAnim->m_fTick - iStart) / iStepTick;
					Vector3 vStart, vEnd, vTrans;
					vStart = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick - 1].vTrans;
					vEnd = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick].vTrans;
					vTrans = Vector3::Lerp(vStart, vEnd, t);
					Vector3 vScale;
					vStart = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick - 1].vScale;
					vEnd = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick].vScale;
					vScale = Vector3::Lerp(vStart, vEnd, t);

					Quaternion q1, q2, qRot;
					q1 = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick - 1].qRot;
					q2 = pGraphics->m_AnimTrackList[pScene->ptAnimTrackIndex.x][iTick].qRot;
					qRot = Quaternion::Slerp(q1, q2, t);

					Matrix matScale = Matrix::CreateScale(vScale);
					Matrix matRotate = Matrix::CreateFromQuaternion(qRot);
					Matrix matTrans = Matrix::CreateTranslation(vTrans);
					m_myNodeList[iNode]->m_pTransform->m_matAnim = matScale * matRotate * matTrans *matParent;
					//pModelObject->m_matAnim = pModelObject->animlist[iTick].mat;

					m_nodeMatList[iNode] = matBiped * m_myNodeList[iNode]->m_pTransform->m_matAnim;
					break;
				}
			}
		}

		Matrix* pMatrices;
		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_pBoneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
		{
			pMatrices = (Matrix*)MappedFaceDest.pData;
			for (int dwObject = 0; dwObject < m_myNodeList.size(); dwObject++)
			{
				//전치 (내적하려고)
				Matrix matAnim = m_nodeMatList[dwObject].Transpose();
				pMatrices[dwObject] = matAnim;
			}
			g_pImmediateContext->Unmap(m_pBoneBuffer.Get(), 0);
		}
	}
	return true;
}
bool myModelObject::Render()
{
	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pBoneBuffer.GetAddressOf());
	m_pTransform->SetMatrix(NULL, 
		&g_pMainCamTransform->m_matView, 
		&g_pMainCamTransform->m_matProj);
	m_pGraphics->m_cbData.vColor[0] = g_pMainCamTransform->m_vLook.x;
	m_pGraphics->m_cbData.vColor[1] = g_pMainCamTransform->m_vLook.y;
	m_pGraphics->m_cbData.vColor[2] = g_pMainCamTransform->m_vLook.z;
	m_pGraphics->Update();
	m_pGraphics->PreRender();
	g_pImmediateContext->IASetInputLayout(m_pGraphics->m_pInputLayout.Get());
	g_pImmediateContext->VSSetConstantBuffers(0, 1, m_pGraphics->m_pConstantBuffer.GetAddressOf());
	g_pImmediateContext->PSSetConstantBuffers(0, 1, m_pGraphics->m_pConstantBuffer.GetAddressOf());
	g_pImmediateContext->VSSetShader(m_pGraphics->m_pVertexShader.Get(), NULL, 0);
	g_pImmediateContext->PSSetShader(m_pGraphics->m_pPixelShader.Get(), NULL, 0);
	g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_pGraphics->m_iTopology);
	for (int iNode = 0; iNode < m_myNodeList.size() ; iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();
		for (int iSub = 0; iSub < pGraphics->m_SubMeshList.size(); iSub++)
		{
			mySubMesh* pMesh = &pGraphics->m_SubMeshList[iSub];
			if (pMesh->m_iFaceCount <= 0) continue;

			ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pVertexBufferIW.Get() };
			UINT iStride[2] = { sizeof(PNCT_VERTEX) ,sizeof(IW_VERTEX) };
			UINT iOffset[2] = { 0,0 };
			g_pImmediateContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);
			g_pImmediateContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			if (pMesh->m_pTexture != nullptr)
			{
				g_pImmediateContext->PSSetShaderResources(0, 1,
					pMesh->m_pTexture->m_pTextureSRV.GetAddressOf());
			}
			if (pMesh->m_pIndexBuffer.Get() == nullptr)
			{
				g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
			}
			else
			{
				g_pImmediateContext->DrawIndexed(pMesh->m_IndexList.size(), 0, 0);
			}
		}
	}

	myGameObject::Render();
	return true;
}
myModelObject::myModelObject()
{
	InsertComponent(new myAnimation);
	//InsertComponent(new myController);
	m_pGraphics = make_shared<myModelGraphics>();
	m_pGraphics->m_pTransform = this->m_pTransform;
	m_pGraphics->m_pGameObject = this;
	m_pAnim = GetComponent<myAnimation>();
	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(Matrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pBoneBuffer.GetAddressOf());
}

myModelObject::~myModelObject()
{
	for (myGameObject* pObj : m_myNodeList)
	{
		pObj->Release();
		delete pObj;
	}
}
