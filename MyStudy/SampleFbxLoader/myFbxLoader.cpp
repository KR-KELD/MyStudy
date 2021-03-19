#include "myFbxLoader.h"

bool myFbxLoader::Init()
{
	m_pSDKManager = FbxManager::Create();
	if (m_pSDKManager == nullptr) return false;
	FbxIOSettings* ios = FbxIOSettings::Create(m_pSDKManager, IOSROOT);
	if (ios == nullptr) return false;
	m_pSDKManager->SetIOSettings(ios);
}

bool myFbxLoader::Release()
{
	return false;
}

myFbxLoader::myFbxLoader()
{

}

myFbxLoader::~myFbxLoader()
{
}
