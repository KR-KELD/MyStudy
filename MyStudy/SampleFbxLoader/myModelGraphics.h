#pragma once
#include "myGraphics.h"

struct myAnimTrack
{
	int iTick;
	Matrix matWorld;
	Vector3 vScale;
	Quaternion qRot;
	Vector3 vTrans;
	myAnimTrack()
	{

	}
};

struct myWeight
{
	vector<int>   nodeIndex;
	vector<float> nodeWeight;
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < nodeIndex.size(); ++i)
		{
			if (fBoneWeight > nodeWeight[i])
			{
				for (DWORD j = (nodeIndex.size() - 1); j > i; --j)
				{
					nodeIndex[j] = nodeIndex[j - 1];
					nodeWeight[j] = nodeWeight[j - 1];
				}
				nodeIndex[i] = iBoneIndex;
				nodeWeight[i] = fBoneWeight;
				break;
			}
		}
	}
	myWeight()
	{
		nodeIndex.resize(8);
		nodeWeight.resize(8);
	}
};
typedef vector<myAnimTrack> AnimTrackList;
class myModelGraphics : public myGraphics
{
public:
	DEFINE_COMPONENT(myModelGraphics, myGraphics, true);
public:
	//주의 인덱스가 순서대로 안쌓이면 애님씬의 인덱스와 틀어진다
	vector<AnimTrackList>		m_AnimTrackList;
	vector<myWeight>			m_WeightList;
	unordered_map<string, Matrix>	m_nodeMatBindPoseMap;
	bool						m_bSkinnedMesh = false;
public:
	virtual bool	CreateInputLayout() override;
	virtual bool	MultiDraw() override;
public:
	myModelGraphics();
	virtual ~myModelGraphics();
};

