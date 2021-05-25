#include "myAnimation.h"
DECLARE_COMPONENT(myAnimation)

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

myAnimScene* myAnimation::GetAnim(wstring strSceneName)
{
	m_AnimSceneIter = m_AnimSceneMap.find(strSceneName);
	if (m_AnimSceneIter == m_AnimSceneMap.end()) return nullptr;
	return &m_AnimSceneIter->second;
}

myAnimation::myAnimation()
{
	m_isUnique = true;

}

myAnimation::~myAnimation()
{
}

