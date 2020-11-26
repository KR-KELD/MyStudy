#pragma once
#include "myStd.h"
class myGrapicsAPI
{
public:
	HDC			m_hScreenDC;
	HDC			m_hOffScreenDC;
	HBITMAP		m_hOffScreenBitmap;
	HBRUSH		m_hBkBrush;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		PreRender();
	bool		PostRender();
	bool		Release();
};

