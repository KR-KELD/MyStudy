#include "mySoundManager.h"

mySound * mySoundManager::Load(const char * filename)
{
	CHAR drive[MAX_PATH] = { 0, };
	CHAR dir[MAX_PATH] = { 0, };
	CHAR name[MAX_PATH] = { 0, };
	CHAR ext[MAX_PATH] = { 0, };
	_splitpath_s(filename, drive, dir, name, ext);

	std::string Dir = dir;
	std::string key;
	std::string loadfile;
	if (Dir.empty())
	{
		key = filename;
	}
	else
	{
		key = name;
		key += ext;
	}
	loadfile = SOUNDPATH;
	loadfile += key;

	mySound* pData = GetPtr(key);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new mySound;
	pData->Init();
	if (pData->Load(m_pSystem, loadfile))
	{

		m_List.insert(make_pair(key, pData));
		return pData;
	}
	return nullptr;
}

mySound * mySoundManager::GetPtr(string filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool mySoundManager::Play(string filename, bool bPaused, bool bLoop)
{
	mySound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->Play(bPaused, bLoop);
		return true;
	}
	return false;
}

bool mySoundManager::PlayEffect(string filename, bool bLoop)
{
	mySound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->PlayEffect(bLoop);
		return true;
	}
	return false;
}

bool mySoundManager::Paused(string filename)
{
	mySound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->Paused();
		return true;
	}
	return false;
}

bool mySoundManager::Stop(string filename)
{
	mySound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->Stop();
		return true;
	}
	return false;
}

bool mySoundManager::SetLoop(string filename, bool bLoop)
{
	mySound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->SetMode(bLoop);
		return true;
	}
	return false;
}

bool mySoundManager::ResetSound(string filename)
{
	mySound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->ResetPos();
		return true;
	}
	return false;
}

bool mySoundManager::Volume(string filename, float fVolume)
{
	mySound* pSound = GetPtr(filename);
	if (pSound != nullptr)
	{
		pSound->Volume(fVolume);
		return true;
	}
	return false;
}

bool mySoundManager::Init()
{
	FMOD_RESULT hr = FMOD::System_Create(&m_pSystem);
	if (hr != FMOD_OK)
	{
		return false;
	}
	if (m_pSystem != nullptr)
	{
		hr = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
		if (hr != FMOD_OK)
		{
			return false;
		}
	}
	return true;
}

bool mySoundManager::Frame()
{
	m_pSystem->update();
	return true;
}

bool mySoundManager::Render()
{
	return true;
}

bool mySoundManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		(*m_iter).second->Release();
		delete (*m_iter).second;
	}
	m_List.clear();

	m_pSystem->close();
	m_pSystem->release();
	return true;
}

mySoundManager::mySoundManager()
{
}

mySoundManager::~mySoundManager()
{
}
