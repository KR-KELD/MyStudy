#include "myCameraManager.h"

bool myCameraManager::SetMainCamera(myGameObject * pCameraObj)
{
	if (pCameraObj != nullptr)
	{
		myCamera* camera = pCameraObj->GetComponent<myCamera>();
		if (camera != nullptr)
		{
			m_pMainCameraObj = pCameraObj;
			m_pMainCamera = camera;
			return true;
		}
	}
	return false;
}

bool myCameraManager::SetMainCamera(wstring strCameraName)
{
	myGameObject* obj = m_CameraContainer.GetGameObject(strCameraName);
	if (SetMainCamera(obj))
	{
		return true;
	}
	return false;
}

bool myCameraManager::Init()
{
	m_CameraContainer.Init();

	return true;
}

bool myCameraManager::Frame()
{
	m_CameraContainer.Frame();
	return true;
}

bool myCameraManager::Render()
{
	//m_CameraContainer.Render();
	return true;
}

bool myCameraManager::Release()
{
	m_CameraContainer.Release();
	return true;
}

myCameraManager::myCameraManager()
{

}

myCameraManager::~myCameraManager()
{

}
