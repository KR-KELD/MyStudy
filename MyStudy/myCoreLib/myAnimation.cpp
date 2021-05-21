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
	//보간 추가적으로 더 하기
	m_AnimSceneIter = m_AnimSceneMap.find(strSceneName);
	if (m_AnimSceneIter == m_AnimSceneMap.end()) return false;
	m_isLerp = isLerp;
	if (m_isLerp)
	{
		m_pPrevScene = m_pCurrentScene;
		m_pCurrentScene = &m_AnimSceneIter->second;
		//m_fPrevTick = 2.0f;
		//m_fLerpTick = 2.0f;
		//m_fLerpTick = min(m_pPrevScene->iLastFrame * m_pPrevScene->iTickPerFrame,
		//	m_fPrevTick + fLerpTime * 
		//	m_pPrevScene->iFrameSpeed * 
		//	m_pPrevScene->iTickPerFrame +
		//	m_pPrevScene->iFirstFrame *
		//	m_pPrevScene->iTickPerFrame);
	/*	m_fTick = m_pCurrentScene->iFirstFrame *m_pCurrentScene->iTickPerFrame +
			(m_fPrevTick - m_pPrevScene->iFirstFrame * m_pPrevScene->iTickPerFrame);*/
	}
	else
	{
		m_pCurrentScene = &m_AnimSceneIter->second;
		//m_fTick = m_pCurrentScene->iFirstFrame *m_pCurrentScene->iTickPerFrame;
		m_fTick = 0.0f;
	}
	return true;
}

myAnimation::myAnimation()
{
	m_isUnique = false;
	m_fTick = 0.0f;
	m_fLerpTick = 0.0f;
	m_fPrevTick = 0.0f;
	m_isLerp = false;
}

myAnimation::~myAnimation()
{
}

