#include "myAnimation.h"
DECLARE_COMPONENT(myAnimation);

bool myAnimation::ChangeAnim(wstring strSceneName)
{
	//���⿡ ���߿� �����߰�
	m_AnimSceneIter = m_AnimSceneMap.find(strSceneName);
	if (m_AnimSceneIter == m_AnimSceneMap.end()) return false;
	m_pCurrentScene = &m_AnimSceneIter->second;
	m_fTick = m_pCurrentScene->iFirstFrame *m_pCurrentScene->iTickPerFrame;
	return true;
}

myAnimation::myAnimation()
{
	m_fTick = 0.0f;
}

myAnimation::~myAnimation()
{
}

