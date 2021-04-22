#pragma once
#include "myModelGraphics.h"
#include "myAnimation.h"

class myModelObject : public myGameObject
{
public:
	DEFINE_COMPONENT(myModelObject, myGameObject, true);
public:

	myAnimation*					m_pAnim;
	shared_ptr<myModelGraphics>		m_pGraphics;
	ComPtr<ID3D11Buffer>			m_pBoneBuffer;
	//������ ���� �ᵵ ������� ���߿� �����ص� ��
	vector<myGameObject*>			m_myNodeList;
	vector<Matrix>					m_nodeMatList;
	//unordered_map<string, Matrix>	m_nodeMatBindPoseMap;
public:
	//bool				SetAnimScene(wstring strSceneName, myAnimScene& anim);
	int					SetAnimTrack(vector<myGameObject*>& nodeList);
	bool				SetAnim(wstring strSceneName, myAnimScene& scene, vector<myGameObject*>& nodeList);
public:					
	virtual bool		Frame() override;
	virtual bool		Render() override;
public:
	myModelObject();
	virtual ~myModelObject();
};

