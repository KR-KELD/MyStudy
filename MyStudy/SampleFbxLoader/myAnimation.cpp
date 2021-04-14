#include "myAnimation.h"
DECLARE_COMPONENT(myAnimation);

bool myAnimation::AddAnim(wstring strSceneName, myAnimScene scene)
{
	m_AnimSceneIter = m_AnimSceneMap.find(strSceneName);
	if (m_AnimSceneIter == m_AnimSceneMap.end())
	{
		m_AnimSceneMap.insert(make_pair(strSceneName, scene));
		return true;
	}
	m_AnimSceneIter->second = scene;
	return true;
}

bool myAnimation::ChangeAnim(wstring strSceneName, bool isLerp, float fLerpTime)
{
	//여기에 나중에 보간추가
	m_AnimSceneIter = m_AnimSceneMap.find(strSceneName);
	if (m_AnimSceneIter == m_AnimSceneMap.end()) return false;
	m_isLerp = isLerp;
	if (m_isLerp)
	{
		m_pPrevScene = m_pCurrentScene;
		m_fPrevTick = m_fTick;
		m_fLerpTick = min(m_pPrevScene->iLastFrame,
			m_fPrevTick + fLerpTime * 
			m_pPrevScene->iFrameSpeed * 
			m_pPrevScene->iTickPerFrame);
	}
	m_pCurrentScene = &m_AnimSceneIter->second;
	
	m_fTick = m_pCurrentScene->iFirstFrame *m_pCurrentScene->iTickPerFrame;
	return true;
}

myAnimation::myAnimation()
{
	m_fTick = 0.0f;
	m_fLerpTick = 0.0f;
	m_fPrevTick = 0.0f;
	m_isLerp = false;
}

myAnimation::~myAnimation()
{
}

