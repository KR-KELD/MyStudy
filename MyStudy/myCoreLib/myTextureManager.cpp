#include "myTextureManager.h"

myTexture * myTextureManager::Load(const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);

	std::wstring Dir = dir;
	std::wstring key;
	std::wstring loadfile;
	if (Dir.empty())
	{
		loadfile = m_szDefaultPath;
		loadfile += filename;
		key = filename;
	}
	else
	{
		loadfile = filename;
		key = name;
		key += ext;
	}
	myTexture* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}

	pData = new myTexture;
	pData->Init();
	if (pData->Load(loadfile))
	{
		m_List.insert(make_pair(key, pData));
		return pData;
	}
	delete pData;
	return nullptr;
}

myTexture * myTextureManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool myTextureManager::Init()
{
	return true;
}

bool myTextureManager::Frame()
{
	return true;
}

bool myTextureManager::Render()
{
	return true;
}

bool myTextureManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		(*m_iter).second->Release();
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}

myTextureManager::myTextureManager()
{
	m_szDefaultPath = L"../../data/object/";
}

myTextureManager::~myTextureManager()
{
	Release();
}
