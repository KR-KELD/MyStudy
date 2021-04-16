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
		//if (m_pAnim->m_isLerp)
		//{
		//	if (m_pAnim->m_fTick +
		//		(m_pAnim->m_pPrevScene->iFirstFrame * m_pAnim->m_pPrevScene->iTickPerFrame)
		//		>= m_pAnim->m_fLerpTick)
		//	{
		//		m_pAnim->m_isLerp = false;
		//	}
		//}
		//if (m_pAnim->m_isLerp)
		//{
		//	m_pAnim->m_fPrevTick += g_fSecondPerFrame *
		//		m_pAnim->m_pPrevScene->iFrameSpeed *
		//		m_pAnim->m_pPrevScene->iTickPerFrame;
		//}
		//if (m_pAnim->m_isLerp)
		//{
		//	m_pAnim->m_fPrevTick -= g_fSecondPerFrame;
		//	if (m_pAnim->m_fPrevTick <= 0.0f)
		//	{
		//		m_pAnim->m_isLerp = false;
		//	}
		//}

		if (m_pAnim->m_fTick + (pScene->iFirstFrame * pScene->iTickPerFrame) >= (pScene->iLastFrame * pScene->iTickPerFrame))
		{
			m_pAnim->m_fTick = 0.0f;
			//m_pAnim->m_fTick = pScene->iFirstFrame *
			//	pScene->iTickPerFrame;
		}
		float fTick = m_pAnim->m_fTick + (pScene->iFirstFrame * pScene->iTickPerFrame);

		for (int iNode = 0; iNode < m_myNodeList.size(); iNode++)
		{
			Matrix matWorld = Matrix::Identity;
			Matrix matParent = Matrix::Identity;
			myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();

			if (m_myNodeList[iNode]->m_pParent != nullptr)
			{
				matParent = m_myNodeList[iNode]->m_pParent->m_pTransform->m_matAnim;
			}

			Vector3 vTrans, vScale;
			Quaternion qRot;
			//pGraphics->m_iTrackIndex = pGraphics->GetTrackIndex(
			//	pScene->iAnimTrackIndex, m_pAnim->m_fTick);
			//if (pGraphics->m_iTrackIndex >= 0)
			//{
			//	int iStart = (*pAnimTrackList)[pGraphics->m_iTrackIndex - 1].iTick;
			//	int iEnd = (*pAnimTrackList)[pGraphics->m_iTrackIndex].iTick;
			//	int iStepTick = iEnd - iStart;
			//	float t = (m_pAnim->m_fTick - iStart) / iStepTick;
			//	Vector3 vStart, vEnd;
			//	vStart = (*pAnimTrackList)[pGraphics->m_iTrackIndex - 1].vTrans;
			//	vEnd = (*pAnimTrackList)[pGraphics->m_iTrackIndex].vTrans;
			//	vTrans = Vector3::Lerp(vStart, vEnd, t);
			//	vStart = (*pAnimTrackList)[pGraphics->m_iTrackIndex - 1].vScale;
			//	vEnd = (*pAnimTrackList)[pGraphics->m_iTrackIndex].vScale;
			//	vScale = Vector3::Lerp(vStart, vEnd, t);
			//	Quaternion q1, q2;
			//	q1 = (*pAnimTrackList)[pGraphics->m_iTrackIndex - 1].qRot;
			//	q2 = (*pAnimTrackList)[pGraphics->m_iTrackIndex].qRot;
			//	qRot = Quaternion::Slerp(q1, q2, t);
			//}
			//if (pGraphics->m_iTrackIndex == -1)
			//{
			//	vTrans = (*pAnimTrackList)[0].vTrans;
			//	vScale = (*pAnimTrackList)[0].vScale;
			//	qRot = (*pAnimTrackList)[0].qRot;
			//}
			pGraphics->GetAnimSRT(pScene->iAnimTrackIndex, fTick
			, vScale, qRot, vTrans);
			//if (m_pAnim->m_isLerp)
			//{
			//	//float fCurrent = m_pAnim->m_fTick - m_pAnim->m_fPrevTick;
			//	//float fTotal = m_pAnim->m_fLerpTick - m_pAnim->m_fPrevTick;
			//	float fPrevTick = m_pAnim->m_fTick + (m_pAnim->m_pPrevScene->iFirstFrame *m_pAnim->m_pPrevScene->iTickPerFrame);
			//	float fRatio = m_pAnim->m_fPrevTick / m_pAnim->m_fLerpTick;
			//	Vector3 vPrevTrans, vPrevScale;
			//	Quaternion qPrevRot;
			//	pGraphics->GetAnimSRT(m_pAnim->m_pPrevScene->iAnimTrackIndex, fPrevTick
			//		, vPrevScale, qPrevRot, vPrevTrans);
			//	vScale = Vector3::Lerp(vScale, vPrevScale, fRatio);
			//	qRot = Quaternion::Slerp(qRot, qPrevRot, fRatio);
			//	vTrans = Vector3::Lerp(vTrans, vPrevTrans, fRatio);
			//}
			Matrix matScale = Matrix::CreateScale(vScale);
			Matrix matRotate = Matrix::CreateFromQuaternion(qRot);
			Matrix matTrans = Matrix::CreateTranslation(vTrans);

			m_myNodeList[iNode]->m_pTransform->m_matAnim = matScale * matRotate * matTrans *matParent;
			m_nodeMatList[iNode] = m_myNodeList[iNode]->m_pTransform->m_matAnim;
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
	m_pGraphics->Update(g_pImmediateContext);
	m_pGraphics->PreRender(g_pImmediateContext);
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
