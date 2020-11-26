#include "myObject.h"

myBitmap * myObject::GetImageInfo()
{
	return m_pImage;
}

bool myObject::SetDest(myPoint p)
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->ptDest = p;
	return true;
}

RECT & myObject::GetImageRect()
{
	return m_rtSrc;
}

bool myObject::SelectTarget(myObjectInfo * pTarget)
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->pTarget = pTarget;
	return true;
}

bool myObject::TargetReset()
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->pTarget = nullptr;
	return true;
}

bool myObject::SetRtSpriteList(MAP_RECT_ARRAY & mapRtSprite)
{
	m_mapRtSpriteList = mapRtSprite;
	return true;
}

MAP_RECT_ARRAY & myObject::GetRtSpriteList()
{
	return m_mapRtSpriteList;
}

bool myObject::SetSpriteRt(wstring strName)
{
	if (m_pObjInfo == nullptr)
		return false;
	m_iterSprite = m_mapRtSpriteList.find(strName);
	if (m_iterSprite != m_mapRtSpriteList.end())
	{
		SetImageRt(m_iterSprite->second[m_pObjInfo->iRectIndex],
				   m_iterSprite->second[m_pObjInfo->iRectIndex]);
		MoveRect(m_pObjInfo->ptPos);
		return true;
	}
	return false;
}

void myObject::DeLinkInfo()
{
	m_pObjInfo = nullptr;
}

void myObject::LinkInfo(myObjectInfo* pObjInfo)
{
	m_pObjInfo = pObjInfo;
	SetImageRt(pObjInfo->rtDest, pObjInfo->rtSrc);
	SetPos(pObjInfo->ptPos);
}

myObjectInfo* myObject::GetInfo()
{
	return m_pObjInfo;
}

void myObject::SetObjBase(myObjBase & _base)
{
	if (!_base.strImageName.empty())
	{
		Load(_base.strImageName);
	}
	if (!_base.strMaskName.empty())
	{
		LoadMask(_base.strMaskName);
	}
	m_ObjType = _base.iObjType;
	m_bTransDraw = _base.bTrans;
	m_dwTransColor = _base.dwTransColor;
	m_mapRtSpriteList = _base.mapRtSpriteList;
	SetAlpha(_base.bAlpha, _base.iAlphaValue);
}

bool myObject::SetPos(myPoint p)
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->ptPos = p;
	MoveRect(p);
	return true;
}

bool myObject::SetImageRt(RECT& rtDest, RECT& rtSrc)
{
	if (rtDest.left != -1) m_pObjInfo->rtDest.left = rtDest.left;
	else m_pObjInfo->rtDest.left = m_rtDest.left;
	if (rtDest.top != -1) m_pObjInfo->rtDest.top = rtDest.top;
	else m_pObjInfo->rtDest.top = m_rtDest.top;
	if (rtDest.right != -1) m_pObjInfo->rtDest.right = rtDest.right;
	else m_pObjInfo->rtDest.right = m_rtDest.right;
	if (rtDest.bottom != -1) m_pObjInfo->rtDest.bottom = rtDest.bottom;
	else m_pObjInfo->rtDest.bottom = m_rtDest.bottom;

	if (rtSrc.left != -1) m_pObjInfo->rtSrc.left = rtSrc.left;
	else m_pObjInfo->rtSrc.left = m_rtSrc.left;
	if (rtSrc.top != -1) m_pObjInfo->rtSrc.top = rtSrc.top;
	else m_pObjInfo->rtSrc.top = m_rtSrc.top;
	if (rtSrc.right != -1) m_pObjInfo->rtSrc.right = rtSrc.right;
	else m_pObjInfo->rtSrc.right = m_rtSrc.right;
	if (rtSrc.bottom != -1) m_pObjInfo->rtSrc.bottom = rtSrc.bottom;
	else m_pObjInfo->rtSrc.bottom = m_rtSrc.bottom;
	return true;
}

bool myObject::MoveRect(myPoint & p)
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->rtDest.left = (LONG)(p.x - (m_pObjInfo->rtDest.right / 2) - g_CameraMgr.GetOffset().x);
	m_pObjInfo->rtDest.top = (LONG)(p.y - (m_pObjInfo->rtDest.bottom / 2) - g_CameraMgr.GetOffset().y);

	m_pObjInfo->rtCollider.left = (LONG)(p.x - (m_pObjInfo->iCollWidth / 2));
	m_pObjInfo->rtCollider.top = (LONG)(p.y - (m_pObjInfo->iCollHeight / 2));
	m_pObjInfo->rtCollider.right = (LONG)(p.x + (m_pObjInfo->iCollWidth / 2));
	m_pObjInfo->rtCollider.bottom = (LONG)(p.y + (m_pObjInfo->iCollHeight / 2));
	return true;
}

bool myObject::SetAlpha(bool bAlpha, int iAlphaValue)
{
	m_bAlpha = bAlpha;
	m_iAlphaValue = iAlphaValue;
	return true;
}

bool myObject::Load(wstring strImageName)
{
	if (!strImageName.empty())
	{
		m_pImage = g_BitmapMgr.Load(strImageName.c_str());
		if (m_pImage != nullptr)
		{
			RECT rtTemp = { 0, 0, m_pImage->m_BitmapInfo.bmWidth,
							m_pImage->m_BitmapInfo.bmHeight };
			m_rtDest = rtTemp;
			m_rtSrc = rtTemp;
			return true;
		}
	}
	return false;
}

bool myObject::LoadMask(wstring strMaskName)
{
	if (!strMaskName.empty())
	{
		m_pMaskImage = g_BitmapMgr.Load(strMaskName.c_str());
		if (m_pMaskImage != nullptr)
		{
			return true;
		}
	}
	return false;
}

void myObject::SetTransition(DWORD dwEvent)
{
}

bool myObject::SpriteProcess()
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->fAttTimer += g_fSecondPerFrame;
	m_pObjInfo->fTimer += g_fSecondPerFrame;
	//스프라이트 일때만 계산
	if (m_pObjInfo->bSprite)
	{
		m_pObjInfo->fTmpTimer += g_fSecondPerFrame;
		if (m_pObjInfo->fLifeTime <= m_pObjInfo->fTimer && m_pObjInfo->fLifeTime > 0.0f)
		{
			m_pObjInfo->bDead = true;
			ResetSprite();
		}
		if (m_pObjInfo->fTmpTimer >= m_pObjInfo->fInterval)
		{
			//왕복 그리기
			if (m_pObjInfo->bRound)
			{
				//순방향 그리기
				if (m_pObjInfo->bIncrease)
				{
					m_pObjInfo->iRectIndex++;
					if (m_pObjInfo->iMaxCount - 1 <= m_pObjInfo->iRectIndex)
					{
						m_pObjInfo->iRectIndex = m_pObjInfo->iMaxCount - 1;
						m_pObjInfo->bIncrease = !m_pObjInfo->bIncrease;
					}
				}
				//역방향 그리기
				else
				{
					m_pObjInfo->iRectIndex--;
					if (0 >= m_pObjInfo->iRectIndex)
					{
						m_pObjInfo->iRectIndex = 0;
						m_pObjInfo->bIncrease = !m_pObjInfo->bIncrease;
						if (m_pObjInfo->bLoop == false)
						{
							m_pObjInfo->bDead = true;
							ResetSprite();
						}
					}
				}
			}
			//단방향 그리기
			else
			{
				m_pObjInfo->iRectIndex++;
				if (m_pObjInfo->iMaxCount <= m_pObjInfo->iRectIndex)
				{
					m_pObjInfo->iRectIndex = 0;
					if (m_pObjInfo->bLoop == false)
					{
						m_pObjInfo->bDead = true;
						ResetSprite();
					}
				}
			}
			m_pObjInfo->fTmpTimer -= m_pObjInfo->fInterval;
		}
	}
	SetSpriteRt(m_pObjInfo->strSpriteName);
	return true;
}

bool myObject::ResetSprite()
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->fTimer = 0.0f;
	m_pObjInfo->fTmpTimer = 0.0f;
	m_pObjInfo->iRectIndex = 0;
	m_pObjInfo->bIncrease = true;

	return true;
}

bool myObject::Move()
{
	return false;
}

bool myObject::Dead()
{
	return false;
}

bool myObject::Attack()
{
	return false;
}

bool myObject::Damage()
{
	return false;
}

bool myObject::MoveSound()
{
	return false;
}

bool myObject::Action()
{
	return true;
}

bool myObject::Action(bool myObject::* fp(int&))
{
	return true;
}

bool myObject::CheckValue(int& iValue)
{
	return true;
}

bool myObject::Reset()
{
	m_pObjInfo = nullptr;
	m_strName.clear();
	//m_pImage = nullptr;
	//m_pMaskImage = nullptr;
	m_dwTransColor = RGB(255, 0, 255);
	m_bAlpha = false;
	m_iAlphaValue = 255;
	m_bTransDraw = false;
	return true;
}

bool myObject::Init()
{
	m_pObjInfo = nullptr;
	m_strName.clear();
	m_pImage = nullptr;
	m_pMaskImage = nullptr;
	m_dwTransColor = RGB(255, 0, 255);
	m_bAlpha = false;
	m_iAlphaValue = 255;
	m_bTransDraw = false;
	return true;
}

bool myObject::Frame()
{
	SpriteProcess();
	return true;
}

bool myObject::Render()
{
	if (m_pImage != nullptr)
	{
		if (!myCollision::InterSectRectWH(m_pObjInfo->rtDest, g_rtClient))
			return false;
		if (m_bAlpha)
		{
			if (m_bTransDraw)
			{
				
			}
			else
			{
				m_pImage->DrawAlphaBlend(m_pObjInfo->rtDest, m_pObjInfo->rtSrc, (BYTE)m_iAlphaValue);
			}
		}
		else
		{
			if (m_bTransDraw)
			{
				m_pImage->DrawTrans(m_pObjInfo->rtDest, m_pObjInfo->rtSrc, m_dwTransColor);
			}
			else
			{
				if (m_pMaskImage != nullptr)
				{
					m_pMaskImage->DrawStretch(m_pObjInfo->rtDest, m_pObjInfo->rtSrc, NORMAL, SRCAND);
					m_pImage->DrawStretch(m_pObjInfo->rtDest, m_pObjInfo->rtSrc, NORMAL, SRCINVERT);
					m_pMaskImage->DrawStretch(m_pObjInfo->rtDest, m_pObjInfo->rtSrc, NORMAL, SRCINVERT);
				}
				else
				{
					m_pImage->DrawStretch(m_pObjInfo->rtDest, m_pObjInfo->rtSrc, NORMAL);
				}
			}
		}
	}
	else
		return false;
	return true;
}

bool myObject::Release()
{
	m_pImage = nullptr;
	m_pMaskImage = nullptr;
	m_mapRtSpriteList.clear();

	return true;
}

myObject::myObject()
{
}

myObject::~myObject()
{
}
