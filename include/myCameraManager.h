#pragma once
#include "myDebugCamera.h"
#include "myModelViewCamera.h"
#include "myController.h"
class myCameraManager : public SingleTon<myCameraManager>
{
	friend class SingleTon<myCameraManager>;
public:
	myGameObject			m_CameraContainer;
	myGameObject*			m_pMainCameraObj;
	myCamera*				m_pMainCamera;
	list<myGameObject*>		m_SubCameraObjList;
public:
	myGameObject*	CreateCameraObj(wstring strObjName, myCamera* pCamera)
	{
		myGameObject* obj = m_CameraContainer.Add(strObjName);
		obj->InsertComponent(pCamera);
		pCamera->Init();
		pCamera->CreateFrustum();
		return obj;
	}
	bool			SetMainCamera(myGameObject* pCameraObj);
	bool			SetMainCamera(wstring strCameraName);
	bool			InsertSubCamera(myGameObject* pCameraObj)
	{
		m_SubCameraObjList.push_back(pCameraObj);
	}
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


