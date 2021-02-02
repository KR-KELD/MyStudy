#pragma once
#include "mySound.h"
#define SOUNDPATH "../../data/sound/"
//#define SOUNDPATH ""
class mySoundManager : public SingleTon<mySoundManager>
{
	friend class SingleTon<mySoundManager>;
private:
	FMOD::System*							m_pSystem;
	std::map<string, mySound*>				m_List;
	std::map<string, mySound*>::iterator	m_iter;
public:
	mySound*    Load(const char* filename);
	mySound*    GetPtr(string filename);
	bool		Play(string filename, bool bPaused, bool bLoop = false);
	bool		PlayEffect(string filename, bool bLoop = false);
	bool		Paused(string filename);
	bool		Stop(string filename);
	bool		SetLoop(string filename, bool bLoop);
	bool		ResetSound(string filename);
	bool		Volume(string filename, float fVolume);
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	mySoundManager();
public:
	~mySoundManager();
};
#define g_SoundMgr mySoundManager::GetInstance()
