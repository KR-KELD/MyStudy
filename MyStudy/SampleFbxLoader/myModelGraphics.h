#pragma once
#include "myGraphics.h"

struct myAnimTrack
{
	int iTick;
	Matrix matWorld;
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

class myModelGraphics : public myGraphics
{
public:
	DEFINE_COMPONENT(myModelGraphics, myGraphics, true);
public:
	vector<myAnimTrack>			m_AnimTrackList;
	vector<myWeight>			m_WeightList;
	bool						m_bSkinnedMesh = false;
	//vector<IW_VERTEX>			m_VertexIWList;
	//ComPtr<ID3D11Buffer>		m_pVertexIWBuffer;
public:
	//virtual bool    CreateVertexBuffer();
	virtual bool	CreateInputLayout();
public:
	myModelGraphics();
	virtual ~myModelGraphics();
};

