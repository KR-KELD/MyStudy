#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include <list>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include "myUtil.h"
#include "myMath.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib, "winmm.lib")

#if NDEBUG
#pragma comment (lib, "myBasic2DLib_R.lib")
#else
#pragma comment (lib, "myBasic2DLib_D.lib")
#endif

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "Msimg32.lib")
using namespace std;
#define SAFE_DELETE(p) {if (p) delete p; (p) = NULL;}
#define WINDOWSTARTX 300
#define WINDOWSTARTY 300
#define WINDOWSIZEX 720
#define WINDOWSIZEY 480
#define BKCOLOR RGB(125,125,125)

struct myKeyMap
{
	bool bCapsLockActive;
};

extern myKeyMap		g_KeyMap;
extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern HDC			g_hScreenDC;
extern HDC			g_hOffScreenDC;
extern RECT			g_rtClient;
extern bool			g_bActive;
extern float		g_fGameTimer;
extern float		g_fSecondPerFrame;

template <class T> class SingleTon
{
public:
	static T& GetInstance()
	{
		static T Single;
		return Single;
	}
};