#include "myFbxObj.h"

bool myFbxObj::ParseMeshSkinning(const FbxMesh* pFbxMesh, SkinData* skindata)
{
	//������ �ִ� ��ü�� �ִ°�
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	//�޽� �������� �� ������ �����´�
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	//���������� ����޴� ��带 8�������� �����ؼ� �Ҵ��Ѵ�
	skindata->Alloc(iVertexCount, 8);

	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		//��Ų������ �ҷ��´�
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		//�����鿡 ������ �ִ� ����� ������ �����´�
		DWORD dwClusterCount = pSkin->GetClusterCount();
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			//ũ������ ������ �����´�
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			//ũ�����Ϳ� ������ �޴� ������ ������ �����´�
			int  dwClusterSize = pCluster->GetControlPointIndicesCount();
			//����� ������ �����´�
			auto pLink = pCluster->GetLink();
			//���Ƿ� �� �ε����� �����ؼ� ��� ������ �����Ѵ�
			int  iBoneIndex = skindata->GetBoneCount();
			skindata->InfluenceNodes.push_back(pLink);
			//������ �޴� �������� �ε����� ����ġ
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int i = 0; i < dwClusterSize; i++)
			{
				//pIndices[i] �� ������
				//iBoneIndex �� ��Ŀ� ������ �޾�
				//pWeights[i] ����ġ�� 
				skindata->InsertWeight(pIndices[i],
					iBoneIndex,
					pWeights[i]);
			}
		}
	}
	return true;
}

void myFbxObj::ParseAnimStack(FbxScene * pFbxScene, FbxString * strAnimStackName)
{
	//�ִϸ��̼� ���� ������ �����´�
	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (anim == nullptr) return;
	//�ִϸ��̼��� ������ �����´�
	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);

	FbxTime FrameTime;
	//1�����ӿ� �ɸ��� �ð��� �����´�?
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
	//
	float fFrameTime = FrameTime.GetSecondDouble();
	float fStartTime, fEndTime;
	//������ ���������� �ҷ����� ������̷� ����
	if (info)
	{
		// scene
		//�ִϸ��̼��� ���۽ð��� ���ð��� �����´�
		fStartTime = info->mLocalTimeSpan.GetStart().GetSecondDouble();
		fEndTime = info->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		FbxTimeSpan tlTimeSpan;
		pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);

		fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
	//���� ������ 30�� �������� ���� ����
	m_Scene.iFirstFrame = fStartTime * 30.0f;
	//�� ������
	m_Scene.iLastFrame = fEndTime * 30.0f;
	//�ʴ� 30������
	m_Scene.iFrameSpeed = 30;
	//�����Ӵ� 160ƽ
	m_Scene.iTickPerFrame = 160;
	m_Scene.iDeltaTick = 1;
	m_Scene.fDeltaTime = fFrameTime * 1.0f;
	//���� �ð�
	m_Scene.fFirstTime = fStartTime;
	//�� �ð�
	m_Scene.fLastTime = fEndTime;
	ParseNodeAnimation(pFbxScene->GetRootNode());
}

void myFbxObj::ParseNodeAnimation(FbxNode * pNode)
{
	if (pNode == nullptr) return;
	if (pNode && (pNode->GetCamera() || pNode->GetLight()))
	{
		return;
	}
#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
	auto anim = m_pFbxScene->GetAnimationEvaluator();
#else
	auto anim = m_pFBXScene->GetEvaluator();
#endif
	float fCurrentTime = 0.0f;
	while (fCurrentTime <= m_Scene.fLastTime)
	{
		FbxTime t;
		//���� �ð��� �޾ƿ´�
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
		myAnimTrack track;
		//�� �ð����� Ʈ�������� ����ؼ�
		track.iTick = fCurrentTime * 30 * 160;
		track.matWorld = DxConvertMatrix(ConvertMatrixA(mat));
		//������Ʈ�� �־��ش�
		auto data = m_MeshList.find(pNode);
		myGraphics* pGraphics = data->second->GetComponent<myGraphics>();
		pGraphics->m_AnimTrackList.push_back(track);
		fCurrentTime += m_Scene.fDeltaTime;
		// mat 1�� �θ���� ����� ���Ѵ�.
		FbxAMatrix self;
		// self 2�� ��� ����( S, R, T )
		// 3�� ���ϸ��̼� Ʈ�� ���
		// S(����), R(���ʹϾ�), T(����)�� ���ø� �ð� �������� ����
	}

	int dwChild = pNode->GetChildCount();
	for (int dwObj = 0; dwObj < dwChild; dwObj++)
	{
		FbxNode* pChildNode = pNode->GetChild(dwObj);
		ParseNodeAnimation(pChildNode);
	}
}

void myFbxObj::ParseAnimation(FbxScene * pFbxScene)
{
	FbxArray<FbxString*>  AnimStackNameArray;
	pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int i = 0; i < iAnimStackCount; i++)
	{
		ParseAnimStack(pFbxScene, AnimStackNameArray.GetAt(i));
	}
}