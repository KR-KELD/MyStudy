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
	//���� �ε����� ������� �Ƚ��̸� �ִԾ��� �ε����� Ʋ������
	vector<AnimTrackList>		m_AnimTrackList;
	int							m_iNextTrackIndex;
	int							m_iTrackIndex;
	vector<myWeight>			m_WeightList;
	unordered_map<string, Matrix>	m_nodeMatBindPoseMap;
	bool						m_bSkinnedMesh = false;
public:
	void			GetAnimSRT(int iAnimIndex, float fTick, Vector3& vScale, Quaternion& qRot, Vector3& vTrans);
	int				GetTrackIndex(int iAnimIndex, float fTick);
public:
	virtual bool	CreateInputLayout() override;
	virtual bool	MultiDraw() override;
public:
	myModelGraphics();
	virtual ~myModelGraphics();
};

