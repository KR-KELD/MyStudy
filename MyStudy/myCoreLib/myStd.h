#pragma once
#include "myBasicStd.h"
#define WINDOWSTARTX 300
#define WINDOWSTARTY 300
#define WINDOWSIZEX 720
#define WINDOWSIZEY 480
#define BKCOLOR RGB(125,125,125)

struct myKeyMap
{
	bool bCapsLockActive;
};

extern myKeyMap			g_KeyMap;
extern HWND				g_hWnd;
extern HINSTANCE		g_hInstance;
extern ID3D11Device*	g_pd3dDevice;
extern HDC				g_hScreenDC;
extern HDC				g_hOffScreenDC;
extern RECT				g_rtClient;
extern bool				g_bActive;
extern float			g_fGameTimer;
extern float			g_fSecondPerFrame;

template <class T> class SingleTon
{
public:
	static T& GetInstance()
	{
		static T Single;
		return Single;
	}
};