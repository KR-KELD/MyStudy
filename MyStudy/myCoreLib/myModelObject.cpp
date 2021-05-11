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

bool myModelObject::Init()
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
	HRESULT hr = g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pBoneBuffer.GetAddressOf());

	m_pNormalTex = g_TextureMgr.m_pWhiteTexture;
	return true;
}

bool myModelObject::Frame()
{
	//myGameObject::PreFrame();
	//myGameObject::Frame();

	myAnimScene* pScene = m_pAnim->m_pCurrentScene;
	if (pScene != nullptr)
	{
		m_pAnim->m_fTick += g_fSecondPerFrame *
			pScene->iFrameSpeed *
			pScene->iTickPerFrame;

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

		float fTick = m_pAnim->m_fTick + (pScene->iFirstFrame * pScene->iTickPerFrame);
		if (fTick >= (pScene->iLastFrame * pScene->iTickPerFrame))
		{
			m_pAnim->m_fTick = 0.0f;
			fTick = pScene->iFirstFrame * pScene->iTickPerFrame;
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
	//임시
	g_pImmediateContext->PSSetShaderResources(1, 1,
		m_pNormalTex->m_pTextureSRV.GetAddressOf());

	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pBoneBuffer.GetAddressOf());
	m_pTransform->SetMatrix(&m_pTransform->m_matWorld, 
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

		Matrix* pMatrices;
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_pBoneBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
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
			g_pImmediateContext->Unmap(m_pBoneBuffer.Get(), 0);
		}
	
		pGraphics->MultiDraw(g_pImmediateContext);
	}

	myGameObject::Render();
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
