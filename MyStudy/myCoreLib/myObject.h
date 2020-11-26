#pragma once
#include "myBitmapManager.h"
#include "myObjectInfo.h"
#include "myCollision.h"
#include "myCameraManager.h"


typedef vector<RECT> RECT_ARRAY;
typedef map<wstring, RECT_ARRAY> MAP_RECT_ARRAY;

struct myObjBase
{
	int					iObjType;
	wstring				strName;
	wstring				strImageName;
	wstring				strMaskName;
	MAP_RECT_ARRAY		mapRtSpriteList;
	bool				bAlpha;
	int					iAlphaValue;
	bool				bTrans;
	DWORD				dwTransColor;
	void Reset()
	{
		iObjType = 0;
		strName.clear();
		strImageName.clear();
		strMaskName.clear();
		mapRtSpriteList.clear();
		bAlpha = false;
		iAlphaValue = 255;
		bTrans = false;
		dwTransColor = 0;
	}
	myObjBase()
	{
		Reset();
	}
};

class myObject
{

protected:
	myObjectInfo*				m_pObjInfo;
	wstring						m_strName;
	int							m_ObjType;
	myBitmap*					m_pImage;
	myBitmap*					m_pMaskImage;
	MAP_RECT_ARRAY				m_mapRtSpriteList;
	MAP_RECT_ARRAY::iterator	m_iterSprite;
	RECT						m_rtDest;
	RECT						m_rtSrc;
	bool						m_bAlpha;
	int							m_iAlphaValue;
	bool						m_bTransDraw;
	DWORD						m_dwTransColor;
public:
	virtual myBitmap*			GetImageInfo();
	virtual bool				SetDest(myPoint p);
	virtual RECT&				GetImageRect();
	virtual bool				SelectTarget(myObjectInfo* pTarget);
	virtual bool				TargetReset();
	virtual bool				SetRtSpriteList(MAP_RECT_ARRAY& mapRtSprite);
	virtual MAP_RECT_ARRAY&		GetRtSpriteList();
	virtual bool				SetSpriteRt(wstring strName);
	virtual void				DeLinkInfo();
	virtual void				LinkInfo(myObjectInfo* pObjInfo);
	virtual myObjectInfo*		GetInfo();
	virtual void				SetObjBase(myObjBase& _base);
	virtual bool				SetPos(myPoint p);
	virtual bool				SetImageRt(RECT& rtDest, RECT& rtSrc);
	virtual bool				MoveRect(myPoint& p);
	virtual bool				SetAlpha(bool bAlpha, int iAlphaValue);
	virtual bool				Load(wstring strImageName);
	virtual bool				LoadMask(wstring strMaskName);
	virtual void				SetTransition(DWORD dwEvent);
	virtual bool				SpriteProcess();
	virtual bool				ResetSprite();
	virtual bool				Move();
	virtual bool				Dead();
	virtual bool				Attack();
	virtual bool				Damage();
	virtual bool				MoveSound();
	virtual bool				Action();
	virtual bool				Action(bool myObject::*fp(int&));
	virtual bool				CheckValue(int& iValue);
public:
	virtual bool				Reset();
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				Render();
	virtual bool				Release();
public:
	myObject();
	virtual ~myObject();
};

