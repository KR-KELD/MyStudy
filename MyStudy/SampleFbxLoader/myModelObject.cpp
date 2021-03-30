#include "myModelObject.h"
#define MAX_BONE_MATRICES 255
DECLARE_COMPONENT(myModelObject);
bool myModelObject::Frame()
{
	myAnimation* pAnim = GetComponent<myAnimation>();
	pAnim->m_fTick += g_fSecondPerFrame *
		pAnim->m_AnimScene.iFrameSpeed *
		pAnim->m_AnimScene.iTickPerFrame;

	if (pAnim->m_fTick >=
		(pAnim->m_AnimScene.iLastFrame * pAnim->m_AnimScene.iTickPerFrame))
	{
		pAnim->m_fTick = 0;
	}

	for (int iNode = 0; iNode < m_myNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		myModelGraphics* pGraphics = m_myNodeList[iNode]->GetComponent<myModelGraphics>();

		Matrix matBiped = Matrix::Identity;
		// 스킨을 바이패드공간으로 이동시킨다
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
				m_nodeMatList[iNode] = matBiped * matWorld;
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
	return true;
}
myModelObject::myModelObject()
{
	InsertComponent(new myAnimation);
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
