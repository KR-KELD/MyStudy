#pragma once
#include "myModelGraphics.h"
#include "myAnimation.h"
#include "myCollider.h"

struct SampleIns
{
	int iID;
	int iNodeIndex;
	bool isActive;
	bool isRender;
	float fTick;
	Vector3 vPos;
	Vector3 vScale;
	Quaternion qRot;
	MY_SPHERE SphereCollider;
	SampleIns()
	{
		isActive = true;
		isRender = true;
		iID = -1;
		iNodeIndex = -1;
		fTick = 0.0f;
	}
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
	//AnimControl
	myAnimScene*				m_pPrevScene;
	float						m_fPrevTick;
	bool						m_isLerp;
	float						m_fLerpTick;
	myAnimScene*				m_pCurrentScene;
	float						m_fTick;
public:
	//�ӽ�
	MY_BOX							m_BoxCollider;
	MY_SPHERE						m_SphereCollider;
public:

	//�ӽ�
	myTexture*						m_pNormalTex;	//����

	myAnimation*					m_pAnim;		//�ִ��� ���� ƽ�� ����
	shared_ptr<myModelGraphics>		m_pGraphics;	//����
	ComPtr<ID3D11Buffer>			m_pBoneBuffer;	//����
	//������ ���� �ᵵ ������� ���߿� �����ص� ��
	vector<myGameObject*>			m_myNodeList;	//����
	vector<Matrix>					m_nodeMatList;
	//unordered_map<string, Matrix>	m_nodeMatBindPoseMap;
	//�ν��Ͻ�
	//ComPtr<ID3D11Buffer>			m_pInstanceBuffer;
	//vector<myBaseInstance>		m_InstanceList;

	//����
	vector<SampleIns>				m_InstanceList;
public:
	int					SetAnimTrack(vector<myGameObject*>& nodeList);
	bool				SetAnim(wstring strSceneName, myAnimScene& scene, vector<myGameObject*>& nodeList);
	bool				ChangeAnim(wstring strSceneName, bool isLerp = false, float fLerpTime = 0.0f);
public:
	virtual bool		Init()	override;
	virtual bool		Frame() override;
	virtual bool		Render(ID3D11DeviceContext*	pd3dContext) override;
public:
	myModelObject();
	virtual ~myModelObject();
};

