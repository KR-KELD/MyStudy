#include "myFbxLoader.h"

myFbxObj * myFbxLoader::Load(const char* szfilename)
{


	CHAR drive[MAX_PATH] = { 0, };
	CHAR dir[MAX_PATH] = { 0, };
	CHAR name[MAX_PATH] = { 0, };
	CHAR ext[MAX_PATH] = { 0, };
	_splitpath_s(szfilename, drive, dir, name, ext);

	std::string Dir = dir;
	std::string key;
	std::string loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += szfilename;
		key = szfilename;
	}
	else
	{
		loadfile = szfilename;
		key = name;
		key += ext;
	}
	myFbxObj* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}

	pData = new myFbxObj(m_pSDKManager);
	if (pData->Load(loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}

myFbxObj * myFbxLoader::GetPtr(string szfilename)
{
	return nullptr;
}

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
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

myFbxLoader::myFbxLoader()
{
	m_szDefaultPath = "../../data";
}

myFbxLoader::~myFbxLoader()
{
}
