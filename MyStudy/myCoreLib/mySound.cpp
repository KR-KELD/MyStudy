#include "mySound.h"

bool mySound::Init()
{
	m_hResult = FMOD_OK;
	return true;
}

bool mySound::Frame()
{
	return true;
}

bool mySound::Render()
{
	return true;
}

bool mySound::Release()
{
	m_pSound->release();
	return true;
}

bool mySound::Load(System * pSystem, std::string szFileName)
{
	m_pSystem = pSystem;
	m_hResult = pSystem->createSound(szFileName.c_str(), FMOD_DEFAULT, 0, &m_pSound);
	if (m_hResult != FMOD_OK)
	{
		return false;
	}
	return true;
}

void mySound::Stop()
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->stop();
	}
}

bool mySound::Play(bool bPaused, bool bLoop)
{
	bool play = false;
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&play);
	}
	if (play == false)
	{
		m_hResult = m_pSystem->playSound(m_pSound,
			NULL, bPaused,
			&m_pChannel);
		if (m_hResult != FMOD_OK)
		{
			return false;
		}
		if (m_pChannel != nullptr)
		{
			m_pChannel->setVolume(0.5f);
		}
		if (SetMode(bLoop) == false)
		{
			return false;
		}
	}
	return true;
}

bool mySound::PlayEffect(bool bLoop)
{
	m_hResult = m_pSystem->playSound(m_pSound,
		NULL, false,
		&m_pChannel);
	if (m_hResult != FMOD_OK)
	{
		return false;
	}
	if (m_pChannel != nullptr)
	{
		m_pChannel->setVolume(0.5f);
	}
	if (SetMode(bLoop) == false)
	{
		return false;
	}
	return true;
}

bool mySound::Paused()
{
	bool paused = false;
	if (m_pChannel != nullptr)
	{
		m_pChannel->getPaused(&paused);
		m_pChannel->setPaused(!paused);
		return !paused;
	}
	return paused;
}

void mySound::Volume(float fVolume)
{
	float fCurrentVolume = 0.0f;
	m_pChannel->getVolume(&fCurrentVolume);
	fCurrentVolume += fVolume * g_fSecondPerFrame;
	fCurrentVolume = min(1.0f, fCurrentVolume);
	fCurrentVolume = max(0.0f, fCurrentVolume);
	m_pChannel->setVolume(fCurrentVolume);
}

void mySound::ResetPos()
{
	m_pChannel->setPosition(0, FMOD_TIMEUNIT_MS);
}

bool mySound::SetMode(bool bLoop)
{
	if (m_pSound != nullptr)
	{
		if (bLoop)
		{
			m_hResult = m_pSound->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			m_hResult = m_pSound->setMode(FMOD_LOOP_OFF);
		}
		return true;
	}
	else
	{
		return false;
	}
}
