#pragma once
#include "myStd.h"
enum ReverseType
{
	NORMAL = 0,
	REVERSE_LR,
	REVERSE_UD,
	REVERSE_ALL
};

class myBitmap
{
public:
	HBITMAP			m_hBitmap;
	HDC				m_hMemDC;
	LPBYTE			m_pRaster;
	BITMAP			m_BitmapInfo;
	BLENDFUNCTION	m_BlendFunction;

public:
	HBITMAP		LoadDIB(wstring strfileName);
	bool		CreateDIB(LONG iWidth, LONG iHeight, int iBitCount = 24);
	bool		Load(wstring strFileName);
	bool		DrawBit(RECT& rtDest, RECT& rtSrc,
		DWORD dwDrawType = SRCCOPY);
	bool		DrawStretch(RECT& rtDest, RECT& rtSrc, 
		ReverseType _Reverse = NORMAL, DWORD dwDrawType = SRCCOPY,
		bool bInter = false);
	bool		DrawTrans(RECT& rtDest, RECT& rtSrc,
		DWORD dwTransColor = RGB(255,0,255));
	bool		DrawTrans(LONG x, LONG y,
		DWORD dwTransColor = RGB(255, 0, 255));
	bool		DrawAlphaBlend(RECT& rtDest, RECT& rtSrc, BYTE alpha = 255);
	bool		DrawAlphaBlend(RECT& rtDest, RECT& rtSrc, BLENDFUNCTION bf);
	bool		DrawAlphaBlend(LONG x, LONG y, BYTE alpha = 255);
	bool		DrawAlphaBlend(LONG x, LONG y, BYTE alpha, BLENDFUNCTION bf);
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
};

