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
	//���븦 �˻��ؼ� �߰��ϰų�
	//�⺻ ���̽��� �����ϰ� ���븸 �����
	//�ð��ڸ��°ŵ� ���� �ؾ��ҵ�
	int iTrackIndex = -1;
	if (nodeList.size() <= 0) return iTrackIndex;
	for (int iNode = 0; iNode < m_myNodeList.size(); iNode++)
	{
		myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();
		bool isBone = false;
		for (int sNode = 0; sNode < nodeList.size(); sNode++)
		{
			if (m_myNodeList[iNode]->m_strName == nodeList[sNode]->m_strName)
			{
				myModelGraphics* pSourceGraphics = nodeList[sNode]->GetComponent<myModelGraphics>();
				pGraphics->m_AnimTrackList.emplace_back(pSourceGraphics->m_AnimTrackList.front());
				isBone = true;
				break;
			}
		}
		
		if (isBone == false)
		{
			pGraphics->m_AnimTrackList.emplace_back(pGraphics->m_AnimTrackList.front());

		}
		if (iTrackIndex < 0)
		{
			iTrackIndex = pGraphics->m_AnimTrackList.size() - 1;
		}
	}
	return iTrackIndex;
}
bool myModelObject::SetAnim(wstring strSceneName, myAnimScene & scene, vector<myGameObject*>& nodeList)
{
	int iTrackIndex = SetAnimTrack(nodeList);
	if (iTrackIndex == -1) return false;
	scene.iAnimTrackIndex = iTrackIndex;
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

		//���� ���������غ��� Ŀ��Ʈ�� ����
		//if (m_pAnim->m_fPrevTick > m_pAnim->m_fLerpTick)
		//{
		//	m_pAnim->m_isLerp = false;
		//}
		//if (m_pAnim->m_isLerp)
		//{
		//	m_pAnim->m_fPrevTick += g_fSecondPerFrame *
		//		m_pAnim->m_pPrevScene->iFrameSpeed *
		//		m_pAnim->m_pPrevScene->iTickPerFrame;
		//}

		if (m_pAnim->m_fTick >=
			(pScene->iLastFrame * pScene->iTickPerFrame))
		{
			m_pAnim->m_fTick = pScene->iFirstFrame *
				pScene->iTickPerFrame;
		}


		for (int iNode = 0; iNode < m_myNodeList.size(); iNode++)
		{
			Matrix matWorld = Matrix::Identity;
			//Matrix matBiped = Matrix::Identity;
			Matrix matParent = Matrix::Identity;
			myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();
			AnimTrackList* pAnimTrackList = &pGraphics->m_AnimTrackList[pScene->iAnimTrackIndex];
			// ��Ų�� �����е�������� �̵���Ų��
			//string szName = to_wm(pGraphics->m_pGameObject->m_strName);


			//auto data = m_nodeMatBindPoseMap.find(szName);
			//if (data != m_nodeMatBindPoseMap.end())
			//{
			//	matBiped = data->second;
			//}

			if (m_myNodeList[iNode]->m_pParent != nullptr)
			{
				matParent = m_myNodeList[iNode]->m_pParent->m_pTransform->m_matAnim;
			}

			Vector3 vTrans, vScale;
			Quaternion qRot;
			if (pGraphics->m_pCurrentTrack != pGraphics->m_pNextTrack)
			{
				//if ((*pAnimTrackList)[pScene->ptAnimTrackIndex.y].iTick <= m_pAnim->m_fTick)
				//{
				//	pScene->ptAnimTrackIndex.y++;
				//	if (pAnimTrackList->size() <= pScene->ptAnimTrackIndex.y)
				//	{
				//		pScene->ptAnimTrackIndex.y = 1;
				//	}
				//}
				int iStart = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y - 1].iTick;
				int iEnd = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y].iTick;
				int iStepTick = iEnd - iStart;
				float t = (m_pAnim->m_fTick - iStart) / iStepTick;
				Vector3 vStart, vEnd;
				vStart = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y - 1].vTrans;
				vEnd = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y].vTrans;
				vTrans = Vector3::Lerp(vStart, vEnd, t);
				vStart = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y - 1].vScale;
				vEnd = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y].vScale;
				vScale = Vector3::Lerp(vStart, vEnd, t);
				Quaternion q1, q2;
				q1 = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y - 1].qRot;
				q2 = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y].qRot;
				qRot = Quaternion::Slerp(q1, q2, t);
			}
			else
			{
				vTrans = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y - 1].vTrans;
				vScale = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y - 1].vScale;
				qRot = (*pAnimTrackList)[pScene->ptAnimTrackIndex.y - 1].qRot;
			}
			Matrix matScale = Matrix::CreateScale(vScale);
			Matrix matRotate = Matrix::CreateFromQuaternion(qRot);
			Matrix matTrans = Matrix::CreateTranslation(vTrans);
			m_myNodeList[iNode]->m_pTransform->m_matAnim = matScale * matRotate * matTrans *matParent;
			m_nodeMatList[iNode] = /*matBiped **/ m_myNodeList[iNode]->m_pTransform->m_matAnim;
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

		//���ε���� ������ ������۸� �ѱ�������
		//if (pGraphics->m_nodeMatBindPoseMap.size() > 0)
		{
			Matrix* pMatrices;
			D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
			if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_pBoneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
			{
				pMatrices = (Matrix*)MappedFaceDest.pData;
				//���ε���� �ִ�(�޽�) ��带 ã�Ƽ� ũ�罺��(������ �ִ� ���)
				//�� ��Ʈ������ ä���� ������۷� �������ش�
				//���� �����ö� �̺κ��� ����
				for (int dwObject = 0; dwObject < m_myNodeList.size(); dwObject++)
				{
					Matrix matBiped = Matrix::Identity;
					string szName = to_wm(m_myNodeList[dwObject]->m_strName);

					auto data = pGraphics->m_nodeMatBindPoseMap.find(szName);
					if (data != pGraphics->m_nodeMatBindPoseMap.end())
					{
						matBiped = data->second;
					}

					//��ġ (�����Ϸ���)
					Matrix matAnim = matBiped * m_nodeMatList[dwObject];
					pMatrices[dwObject] = matAnim.Transpose();
				}
				g_pImmediateContext->Unmap(m_pBoneBuffer.Get(), 0);
			}
		}

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
