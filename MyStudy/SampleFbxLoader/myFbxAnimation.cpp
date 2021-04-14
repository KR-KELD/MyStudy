#include "myFbxObj.h"

bool myFbxObj::ParseMeshSkinningMap(const FbxMesh* pFbxMesh, vector<myWeight>& skindata, myModelGraphics* pGraphics)
{
	//영향을 주는 객체가 있는가
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	//메쉬 제어점의 총 갯수를 가져온다
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	//제어점마다 영향받는 노드를 8개까지만 지정해서 할당한다
	skindata.resize(iVertexCount);

	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		//스킨정보를 불러온다
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		//정점들에 영향을 주는 노드의 갯수를 가져온다
		DWORD dwClusterCount = pSkin->GetClusterCount();
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			//크러스터 정보를 가져온다
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			//뼈대공간으로 보내주는 매트릭스
			FbxAMatrix matXBindPose;
			//로컬공간으로 보내주는 매트릭스
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			//월드에 있는 정점을 원점으로 보냈다가 뼈대공간으로 보내주는 매트릭스
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;
			//뼈대공간에 있는 정점을 원점으로 모아주는 매트릭스
			Matrix matInvBindPos = DxConvertMatrix(ConvertMatrixA(matBindPose));
			matInvBindPos = matInvBindPos.Invert();

			pGraphics->m_nodeMatBindPoseMap.insert(
				make_pair(pCluster->GetLink()->GetName(), matInvBindPos));

			//크러스터에 영향을 받는 정점의 갯수를 가져온다
			int  dwClusterSize = pCluster->GetControlPointIndicesCount();
			//노드의 정보를 가져온다
			auto data = m_pNodeIndexMap.find(pCluster->GetLink());
			//임의로 본 인덱스를 설정해서 노드 정보를 연결한다
			int  iBoneIndex = data->second;

			//영향을 받는 정점들의 인덱스와 가중치
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			for (int i = 0; i < dwClusterSize; i++)
			{
				//pIndices[i] 번 정점이
				//iBoneIndex 번 행렬에 영향을 받아
				//pWeights[i] 가중치로 
				skindata[pIndices[i]].InsertWeight(iBoneIndex, pWeights[i]);
			}
		}
	}
	return true;
}

void myFbxObj::ParseAnimStack(FbxScene * pFbxScene, FbxString * strAnimStackName)
{
	//애니메이션 스택 정보를 가져온다
	FbxAnimStack* anim = pFbxScene->FindMember<FbxAnimStack>(strAnimStackName->Buffer());
	if (anim == nullptr) return;
	//애니메이션의 정보를 가져온다
	FbxTakeInfo* info = pFbxScene->GetTakeInfo(*strAnimStackName);

	FbxTime FrameTime;
	//1프레임에 걸리는 시간을 가져온다?
	FrameTime.SetTime(0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode());
	//
	float fFrameTime = FrameTime.GetSecondDouble();
	float fStartTime, fEndTime;
	//인포의 유무에따라 불러오는 방식차이로 추정
	if (info)
	{
		// scene
		//애니메이션의 시작시간과 끝시간을 가져온다
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
	//myAnimation* pAnim = m_pModelObject->GetComponent<myAnimation>();
	//시작 프레임
	m_AnimScene.iFirstFrame = fStartTime * 30.0f;   //0.1초  0.1 x 30 프레임
	//끝 프레임
	m_AnimScene.iLastFrame = fEndTime * 30.0f;		//1초 1 x 30 프레임
	//초당 30프레임
	m_AnimScene.iFrameSpeed = 30;
	//프레임당 160틱
	m_AnimScene.iTickPerFrame = 160;
	m_AnimScene.iDeltaTick = 1;
	m_AnimScene.fDeltaTime = fFrameTime * 1.0f;
	//시작 시간
	m_AnimScene.fFirstTime = fStartTime;
	//끝 시간
	m_AnimScene.fLastTime = fEndTime;
	//fbx 내장애니메이션일경우 가장 먼저 만들어지기때문에
	//스택이 0번이 된다
	m_AnimScene.iAnimTrackIndex = 0;
	//ParseNodeAnimation(pFbxScene->GetRootNode());
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
	auto data = m_pNodeMap.find(pNode);
	myModelGraphics* pGraphics = data->second->GetComponent<myModelGraphics>();
	pGraphics->m_AnimTrackList.push_back(AnimTrackList());
	float fCurrentTime = 0.0f;
	while (fCurrentTime <= m_AnimScene.fLastTime)
	{
		FbxTime t;
		//현재 시간을 받아온다
		t.SetSecondDouble(fCurrentTime);
		FbxAMatrix mat = anim->GetNodeGlobalTransform(pNode, t);
		myAnimTrack track;
		//그 시간대의 트랙정보를 계산해서
		track.iTick = fCurrentTime * 30 * 160;
		track.matWorld = DxConvertMatrix(ConvertMatrixA(mat));
		//오브젝트에 넣어준다

		pGraphics->m_AnimTrackList.back().push_back(track);
		fCurrentTime += m_AnimScene.fDeltaTime;
		// mat 1차 부모행렬 역행렬 곱한다.
		FbxAMatrix self;
		// self 2차 행렬 분해( S, R, T )
		// 3차 에니메이션 트랙 행렬
		// S(벡터), R(쿼터니언), T(벡터)를 샘플링 시간 간격으로 저장
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