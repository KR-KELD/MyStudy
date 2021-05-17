#pragma once
#include "myBasicStd.h"
#include <wrl.h>
#include "myStaticFunc.h"
//#include <Windows.Foundation.h>
//#include <wrl\wrappers\corewrappers.h>
//#include <wrl\client.h>
//using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
//using namespace Microsoft::WRL::Wrappers;
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

template <class T> class SingleTon
{
public:
	static T& GetInstance()
	{
		static T Single;
		return Single;
	}
};


/////////////////////////////////////////  반환하지 않는다. ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H
//주의사항:매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행 되지 않게 x=DX메소드만 사용해야 한다.
#define H(x){ hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__), __LINE__);\
							MessageBox(NULL, buffer,output,MB_OK);}\
						}
#endif
#else
#ifndef H
#define H(x) (x)
#endif
#endif

/////////////////////////////////////////  실패의 경우만 반환한다 ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H_RETURN
//주의사항:hr = (x); 이 있어야 매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행하지 않는다.
#define H_RETURN(x){  hr = (x); if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
						}
#endif
#else
#define H_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; }}
#endif

/////////////////////////////////////////  무조건 반환한다 ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef U_RETURN
//주의사항:hr = (x); 이 있어야 매크로 함수안에서 매크로 함수(x가 함수로 해석)를 중복 실행하지 않는다.
#define U_RETURN(x){  hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
							else { return hr;}\
						}
#endif
#else
#define U_RETURN(x){ hr = (x); return hr; }
#endif


#ifndef V
#define V(x)       { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef T_RETURN
#define T_RETURN(x,ret){ hr = (x); if( FAILED(hr) ) { return ret; } }
#endif


//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif