#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "myUtil.h"
#include "myMath.h"
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
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