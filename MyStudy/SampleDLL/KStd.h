#pragma once
#ifdef SAMPLEDLL_EXPORTS
#define SAMPLEDLL_API __declspec(dllexport)
#else
#define SAMPLEDLL_API __declspec(dllimport)
#endif

#include "KBasicStd.h"

#define WINDOWSTARTX 300
#define WINDOWSTARTY 300
#define WINDOWSIZEX 720
#define WINDOWSIZEY 480
#define BKCOLOR RGB(125,125,125)
static const TCHAR* DataFolderPath = L"../../data/";

struct KKeyMap
{
	bool bCapsLockActive;
};