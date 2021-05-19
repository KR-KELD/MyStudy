#pragma once
#include "myModelGraphics.h"
#include "myAnimation.h"

struct SampleIns
{
	int iID;
	bool isActive;
	float fTick;
	Vector3 vPos;
	Vector3 vScale;
	Quaternion qRot;
	MY_BOX	myBoxCollider;
	Matrix GetWorld()
	{
		Matrix matWorld = Matrix::Identity;
		Matrix scale = Matrix::CreateScale(vScale);
		Matrix rot = Matrix::CreateFromQuaternion(qRot);
		matWorld = scale * rot;
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;
		return matWorld;
	}
};

class myModelObject : public myGameObject
{
public:
	DEFINE_COMPONENT(myModelObject, myGameObject, true);
public:
	//임시
	MY_BOX							m_myBoxCollider;
public:
	int								m_iObjectID;
	//임시
	myTexture*						m_pNormalTex;

	myAnimation*					m_pAnim;
	shared_ptr<myModelGraphics>		m_pGraphics;
	ComPtr<ID3D11Buffer>			m_pBoneBuffer;
	//기존의 맵을 써도 상관없음 나중에 수정해도 됨
	vector<myGameObject*>			m_myNodeList;
	vector<Matrix>					m_nodeMatList;
	//unordered_map<string, Matrix>	m_nodeMatBindPoseMap;
	//인스턴싱
	//ComPtr<ID3D11Buffer>			m_pInstanceBuffer;
	//vector<myBaseInstance>		m_InstanceList;

	//샘플
	vector<SampleIns>				m_InstanceList;
public:
	int					SetAnimTrack(vector<myGameObject*>& nodeList);
	bool				SetAnim(wstring strSceneName, myAnimScene& scene, vector<myGameObject*>& nodeList);
public:					
	virtual bool		Init()	override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
public:
	myModelObject();
	virtual ~myModelObject();
};

