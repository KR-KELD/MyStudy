#pragma once
#include "myGraphics.h"
#include "fbxsdk.h"

enum class myEClassType
{
	CLASS_GEOM = 0,
	CLASS_BONE,

};

struct myAnimMatrix
{
	Matrix matAnim[255];
};

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
	myTexture*					m_pNormalTex;
	vector<AnimTrackList>		m_AnimTrackList;
	int							m_iNextTrackIndex;
	int							m_iTrackIndex;
	vector<myWeight>			m_WeightList;
	unordered_map<string, Matrix>	m_nodeMatBindPoseMap;
	bool						m_bSkinnedMesh = false;

	//추가-바인드포즈벡터화-
	myAnimMatrix				m_matAnimList;
	//vector<Matrix>				m_BindPoseList;
	//vector<FbxNode*>			m_pFbxNodeList;
	//FbxNode*					m_pFbxNode;
	//vector<myModelGraphics*>	m_pMeshLinkList;
	ComPtr<ID3D11Buffer>		m_pAnimCB;
	myEClassType				m_eClassType;
public:
	std::vector<mySubMesh2>		m_SubMeshList2;
	std::vector<PNCT2_VERTEX>	m_VertexList2;
public:
	void			GetAnimSRT(int iAnimIndex, float fTick, Vector3& vScale, Quaternion& qRot, Vector3& vTrans);
	void			GetAnimSRT_NoneLerp(int iAnimIndex, float fTick, Vector3& vScale, Quaternion& qRot, Vector3& vTrans);
	int				GetTrackIndex(int iAnimIndex, float fTick);
	virtual void	CreateTangentData(Vector3 *v1, Vector3 *v2, Vector3 *v3,
		Vector2 uv1, Vector2 uv2, Vector2 uv3,
		Vector3 *vTangent, Vector3 *vBiNormal, Vector3 *vNormal);
	virtual void	CreateTangentData(Vector3 *v1, Vector3 *v2, Vector3 *v3,
		Vector2 uv1, Vector2 uv2, Vector2 uv3,Vector3 *vNormal,
		Vector3 *vTangent);
	virtual bool	CreateAnimCBBuffer();
public:
	virtual bool	CreateInputLayout() override;
	virtual bool	MultiDraw(ID3D11DeviceContext*	pd3dContext) override;
public:
	myModelGraphics();
	virtual ~myModelGraphics();
};

