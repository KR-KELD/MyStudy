#pragma once
#include "myBasicStd.h"

//#include <Windows.Foundation.h>
//#include <wrl\wrappers\corewrappers.h>
//#include <wrl\client.h>
//using namespace ABI::Windows::Foundation;

//using namespace Microsoft::WRL::Wrappers;

#include "myStaticFunc.h" //basicstd Æ÷ÇÔ

#define WINDOWSTARTX 300
#define WINDOWSTARTY 300
#define WINDOWSIZEX 720
#define WINDOWSIZEY 480
#define BKCOLOR RGB(125,125,125)
static const TCHAR* DataFolderPath = L"../../data/";

struct myKeyMap
{
	bool bCapsLockActive;
};

extern myKeyMap				g_KeyMap;
extern HWND					g_hWnd;
extern HINSTANCE			g_hInstance;
extern ID3D11Device*		g_pd3dDevice;
extern ID3D11DeviceContext*	g_pImmediateContext;
extern HDC					g_hScreenDC;
extern HDC					g_hOffScreenDC;
extern RECT					g_rtClient;
extern bool					g_bActive;
extern float				g_fGameTimer;
extern float				g_fSecondPerFrame;

