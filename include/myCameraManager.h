#pragma once
#include "myDebugCamera.h"
#include "myModelViewCamera.h"
#include "myController.h"
class myCameraManager : public SingleTon<myCameraManager>
{
	friend class SingleTon<myCameraManager>;
public:
	//ī�޶���� ��� �����̳�
	myGameObject			m_CameraContainer;
	//����ī�޶� ������Ʈ
	myGameObject*			m_pMainCameraObj;
	//����ī�޶� ������Ʈ
	myCamera*				m_pMainCamera;
	//����ī�޶� ����Ʈ
	list<myGameObject*>		m_SubCameraObjList;
public:
	//ī�޶� ��ü�� �����ϰ� �����̳ʿ� ����
	myGameObject*	CreateCameraObj(wstring strObjName, myCamera* pCamera)
	{
		myGameObject* obj = m_CameraContainer.Add(strObjName);
		obj->InsertComponent(pCamera);
		pCamera->Init();
		pCamera->CreateFrustum();
		return obj;
	}
	//����ī�޶� ����(��ü ������)
	bool			SetMainCamera(myGameObject* pCameraObj);
	//����ī�޶� ����(�����̳� �̸� �˻�)
	bool			SetMainCamera(wstring strCameraName);
	//����ī�޶� ���
	bool			InsertSubCamera(myGameObject* pCameraObj)
	{
		m_SubCameraObjList.push_back(pCameraObj);
	}
	//����ī�޶� ����
	bool			RemoveSubCamera(myGameObject* pCameraObj)
	{
		m_SubCameraObjList.remove(pCameraObj);
	}

public:
	bool	Init();
	bool	PreFrame();
	bool	Frame();
	bool	PostFrame();
	bool	PreRender();
	bool	Render();
	bool	PostRender();
	bool	Release();
public:
	myCameraManager();
	~myCameraManager();
};
#define g_CamMgr myCameraManager::GetInstance()
#define g_pMainCamTransform myCameraManager::GetInstance().m_pMainCamera->m_pTransform
#define g_CamList myCameraManager::GetInstance().m_CameraContainer


