#include "myModelObject.h"
#include "myCameraManager.h"

#define MAX_BONE_MATRICES 255
DECLARE_COMPONENT(myModelObject);

int myModelObject::SetAnimTrack(vector<myGameObject*>& nodeList)
{
	//뼈대를 검색해서 추가하거나
	//기본 베이스를 복사하고 뼈대만 덮어쓰기
	//시간자르는거도 따로 해야할듯
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

bool myModelObject::ChangeAnim(wstring strSceneName, bool isLerp, float fLerpTime)
{
	//여기에 나중에 보간추가
	//보간 추가적으로 더 하기
	myAnimScene* pScene = m_pAnim->GetAnim(strSceneName);
	if (pScene == nullptr) return false;
	if (pScene == m_pCurrentScene) return false;
	m_isLerp = isLerp;
	if (m_isLerp)
	{
		m_pPrevScene = m_pCurrentScene;
		m_pCurrentScene = pScene;
		//m_fPrevTick = 2.0f;
		//m_fLerpTick = 2.0f;
		//m_fLerpTick = min(m_pPrevScene->iLastFrame * m_pPrevScene->iTickPerFrame,
		//	m_fPrevTick + fLerpTime * 
		//	m_pPrevScene->iFrameSpeed * 
		//	m_pPrevScene->iTickPerFrame +
		//	m_pPrevScene->iFirstFrame *
		//	m_pPrevScene->iTickPerFrame);
	/*	m_fTick = m_pCurrentScene->iFirstFrame *m_pCurrentScene->iTickPerFrame +
			(m_fPrevTick - m_pPrevScene->iFirstFrame * m_pPrevScene->iTickPerFrame);*/
	}
	else
	{
		m_pCurrentScene = pScene;
		//m_fTick = m_pCurrentScene->iFirstFrame *m_pCurrentScene->iTickPerFrame;
		m_fTick = 0.0f;
	}
	return true;
}

bool myModelObject::Init()
{
	myGameObject::Init();
	m_pCurrentScene = nullptr;
	InsertComponent(new myAnimation);
	m_pGraphics = make_shared<myModelGraphics>();
	m_pGraphics->m_pTransform = this->m_pTransform;
	m_pGraphics->m_pGameObject = this;
	//추가-바인드포즈벡터화-
	//m_pGraphics->CreateAnimCBBuffer();
	m_pAnim = GetComponent<myAnimation>();

	InsertComponent(new mySphereCollider);
	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(Matrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	HRESULT hr = g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pBoneBuffer.GetAddressOf());
	m_pNormalTex = g_TextureMgr.m_pWhiteTexture;
	

	m_fTick = 0.0f;
	m_fLerpTick = 0.0f;
	m_fPrevTick = 0.0f;
	m_isLerp = false;
	return true;
}

bool myModelObject::Frame()
{
	myGameObject::Frame();


	if (m_pCurrentScene != nullptr)
	{
		m_fTick += g_fSecondPerFrame *
			m_pCurrentScene->iFrameSpeed *
			m_pCurrentScene->iTickPerFrame;

		//보간 생각정리해보기 커런트랑 같이
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

		float fTick = m_fTick + (m_pCurrentScene->iFirstFrame * m_pCurrentScene->iTickPerFrame);
		if (fTick >= (m_pCurrentScene->iLastFrame * m_pCurrentScene->iTickPerFrame))
		{
			m_fTick = 0.0f;
			fTick = m_pCurrentScene->iFirstFrame * m_pCurrentScene->iTickPerFrame;
			//m_pAnim->m_fTick = pScene->iFirstFrame *
			//	pScene->iTickPerFrame;
		}

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

			pGraphics->GetAnimSRT(m_pCurrentScene->iAnimTrackIndex, fTick
			, vScale, qRot, vTrans);

			//pGraphics->GetAnimSRT_NoneLerp(m_pCurrentScene->iAnimTrackIndex, fTick
			//	, vScale, qRot, vTrans);
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
bool myModelObject::PreRender(ID3D11DeviceContext * pd3dContext)
{
	//임시
	pd3dContext->PSSetShaderResources(1, 1,
		m_pNormalTex->m_pTextureSRV.GetAddressOf());


	pd3dContext->VSSetConstantBuffers(1, 1, m_pBoneBuffer.GetAddressOf());
	m_pGraphics->m_cbData.vColor[0] = g_pMainCamTransform->m_vLook.x;
	m_pGraphics->m_cbData.vColor[1] = g_pMainCamTransform->m_vLook.y;
	m_pGraphics->m_cbData.vColor[2] = g_pMainCamTransform->m_vLook.z;
	m_pGraphics->Update(pd3dContext);
	m_pGraphics->PreRender(pd3dContext);
	pd3dContext->IASetInputLayout(m_pGraphics->m_pInputLayout.Get());
	pd3dContext->VSSetConstantBuffers(0, 1, m_pGraphics->m_pConstantBuffer.GetAddressOf());
	pd3dContext->PSSetConstantBuffers(0, 1, m_pGraphics->m_pConstantBuffer.GetAddressOf());
	pd3dContext->VSSetShader(m_pGraphics->m_pVertexShader.Get(), NULL, 0);
	pd3dContext->PSSetShader(m_pGraphics->m_pPixelShader.Get(), NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_pGraphics->m_iTopology);
	return true;
}
bool myModelObject::PostRender(ID3D11DeviceContext * pd3dContext)
{
	for (int iNode = 0; iNode < m_myNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;

		myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();
		//추가-바인드포즈벡터화-
		if (pGraphics->m_eClassType != myEClassType::CLASS_GEOM) continue;
		//for (int dwObject = 0; dwObject < pGraphics->m_BindPoseList.size(); dwObject++)
		//{
		//	pGraphics->m_matAnimList.matAnim[dwObject] =
		//		pGraphics->m_BindPoseList[dwObject] *
		//		pGraphics->m_pMeshLinkList[dwObject]->m_pTransform->m_matAnim;
		//	pGraphics->m_matAnimList.matAnim[dwObject].Transpose();
		//}
		//pd3dContext->UpdateSubresource(pGraphics->m_pAnimCB.Get(), 0, NULL,
		//	&pGraphics->m_matAnimList, 0, 0);
		//pd3dContext->VSSetConstantBuffers(1, 1, pGraphics->m_pAnimCB.GetAddressOf());


		Matrix* pMatrices;
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(pd3dContext->Map((ID3D11Resource*)m_pBoneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
		{
			pMatrices = (Matrix*)MappedFaceDest.pData;
			//바인드포즈가 있는(메시) 노드를 찾아서 크루스터(영향을 주는 노드)
			//의 매트릭스를 채워서 상수버퍼로 전달해준다
			//뼈대 가져올때 이부분을 참고

			for (int dwObject = 0; dwObject < m_myNodeList.size(); dwObject++)
			{
				Matrix matBiped = Matrix::Identity;
				string szName = to_wm(m_myNodeList[dwObject]->m_strName);

				auto data = pGraphics->m_nodeMatBindPoseMap.find(szName);
				if (data != pGraphics->m_nodeMatBindPoseMap.end())
				{
					matBiped = data->second;
				}

				//전치 (내적하려고)
				Matrix matAnim = matBiped * m_nodeMatList[dwObject];
				pMatrices[dwObject] = matAnim.Transpose();
			}
			pd3dContext->Unmap(m_pBoneBuffer.Get(), 0);
		}

		pGraphics->MultiDraw(pd3dContext);
	}
	myGameObject::Render(pd3dContext);
	return true;
}
bool myModelObject::Render(ID3D11DeviceContext*	pd3dContext)
{
	PreRender(pd3dContext);
	PostRender(pd3dContext);
	return true;
}
myModelObject::myModelObject()
{

}

myModelObject::~myModelObject()
{
	for (myGameObject* pObj : m_myNodeList)
	{
		pObj->Release();
		delete pObj;
	}
}
