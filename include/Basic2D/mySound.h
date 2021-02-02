#pragma once
#include "myStd.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
using namespace FMOD;

class mySound
{
public:
	FMOD::System*		m_pSystem;
	FMOD::Sound*		m_pSound;
	FMOD::Channel*		m_pChannel;
	FMOD_RESULT			m_hResult;
	TCHAR				m_szBuffer[256];
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
	bool			Load(System* pSystem, std::string szFileName);
	void			Stop();
	bool			Play(bool bPaused = false, bool bLoop = true);
	bool			PlayEffect(bool bLoop = true);
	bool			Paused();
	void			Volume(float fVolume);
	void			ResetPos();
	bool			SetMode(bool bLoop);
};

