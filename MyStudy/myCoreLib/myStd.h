#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <string>
#include <tchar.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include "myMath.h"
#pragma comment(lib, "winmm.lib")

#if NDEBUG
#pragma comment (lib, "myCoreLib_R.lib")
#else
#pragma comment (lib, "myCoreLib_D.lib")
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
struct myPoint
{
	float x;
	float y;
	myPoint() = default;
	myPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	myPoint operator - (myPoint p)
	{
		myPoint pt;
		pt.x = x - p.x;
		pt.y = y - p.y;
		return pt;
	}
	bool operator == (myPoint& p)
	{
		if (FLOAT_EQUAL(x, p.x))
		{
			if (FLOAT_EQUAL(y, p.y))
			{
				return true;
			}
		}
		return false;
	}
	bool operator != (myPoint& p)
	{
		if (FLOAT_EQUAL(x, p.x))
		{
			if (FLOAT_EQUAL(y, p.y))
			{
				return false;
			}
		}
		return true;
	}
	myPoint operator + (myPoint p)
	{
		myPoint pt;
		pt.x = x + p.x;
		pt.y = y + p.y;
		return pt;
	}

	myPoint operator + (float fValue)
	{
		myPoint pt;
		pt.x = x + fValue;
		pt.y = y + fValue;
		return pt;
	}

	myPoint operator * (float fValue)
	{
		myPoint pt;
		pt.x = x * fValue;
		pt.y = y * fValue;
		return pt;
	}

	myPoint operator * (myPoint p)
	{
		myPoint pt;
		pt.x = x * p.x;
		pt.y = y * p.y;
		return pt;
	}

	float Length()
	{
		return sqrt(x * x + y * y);
	}

	myPoint Normalize()
	{
		float fLength = Length();
		if (fLength == 0.0f)
			fLength = EPSILON;
		x /= fLength;
		y /= fLength;
		return myPoint(x, y);
	}

	float GetAngle(myPoint& pA, myPoint& pB)
	{
		myPoint pt = pB - pA;
		float dist = pt.Length();
		float angle = acosf(pt.x / dist);

		if (pt.y > 0)
			angle = PI2M - angle;
		return angle;
	}
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