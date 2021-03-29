#include "myFbxObj.h"

bool myFbxObj::ParseMeshSkinningMap(const FbxMesh* pFbxMesh, vector<myWeight>& skindata)
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
	skindata.resize(iVertexCount);

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
			auto data = m_pNodeMap.find(pCluster->GetLink());
			//���Ƿ� �� �ε����� �����ؼ� ��� ������ �����Ѵ�
			int  iBoneIndex = data->second;
			//������ �޴� �������� �ε����� ����ġ
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int i = 0; i < dwClusterSize; i++)
			{
				//pIndices[i] �� ������
				//iBoneIndex �� ��Ŀ� ������ �޾�
				//pWeights[i] ����ġ�� 
				skindata[pIndices[i]].InsertWeight(iBoneIndex, pWeights[i]);
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
	//���� ������
	m_Animation.m_AnimScene.iFirstFrame = fStartTime * 30.0f;   //0.1��  0.1 x 30 ������
	//�� ������
	m_Animation.m_AnimScene.iLastFrame = fEndTime * 30.0f;		//1�� 1 x 30 ������
	//�ʴ� 30������
	m_Animation.m_AnimScene.iFrameSpeed = 30;
	//�����Ӵ� 160ƽ
	m_Animation.m_AnimScene.iTickPerFrame = 160;
	m_Animation.m_AnimScene.iDeltaTick = 1;
	m_Animation.m_AnimScene.fDeltaTime = fFrameTime * 1.0f;
	//���� �ð�
	m_Animation.m_AnimScene.fFirstTime = fStartTime;
	//�� �ð�
	m_Animation.m_AnimScene.fLastTime = fEndTime;
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
	while (fCurrentTime <= m_Animation.m_AnimScene.fLastTime)
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
		myModelGraphics* pGraphics = data->second->GetComponent<myModelGraphics>();
		pGraphics->m_AnimTrackList.push_back(track);
		fCurrentTime += m_Animation.m_AnimScene.fDeltaTime;
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