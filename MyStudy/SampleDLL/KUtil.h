#pragma once
#include <string>
#include <atlconv.h> // A2W
#include <vector>

typedef std::basic_string<TCHAR>		T_STR;
typedef std::basic_string<CHAR>			C_STR;
typedef std::vector<T_STR>				T_STR_VECTOR;

static void PRINT(char* fmt, ...) // 나열연산자
{
	va_list arg;
	va_start(arg, fmt);
	char buf[256] = { 0, };
	vsprintf_s(buf, fmt, arg);
	printf("\n=====> %s", buf);
	va_end(arg);
}
//멀티바이트->유니코드
static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};
//유니코드->멀티바이트
static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};
static char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  소스
		retData, iLength, // 대상
		NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		src, -1,  //  소스
		pDest, iLength, // 대상
		NULL, NULL);
	if (iRet == 0) return false;
	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  소스
		retData, iLength); // 대상
	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1,  //  소스
		pDest, iLength); // 대상		
	if (iRet == 0) return false;
	return true;
}