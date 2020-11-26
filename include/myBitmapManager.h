#pragma once
#include "myBitmap.h"
#define BITMAPPATH L"../../data/bitmap/"
//#define BITMAPPATH L""
class myBitmapManager : public SingleTon<myBitmapManager>
{
	friend class SingleTon<myBitmapManager>;
private:
	std::map<wstring, myBitmap*>				m_List;
	std::map<wstring, myBitmap*>::iterator		m_iter;
public:
	myBitmap*		Load(const TCHAR* szFileName);
	myBitmap*		GetPtr(wstring strFileName);
	myBitmap*		CreateLoad(LONG iWidth, LONG iHeight, const TCHAR* szFileName, int iBitCount = 24);
	static HBITMAP  CreateDIB(LONG iWidth, LONG iHeight, LPBYTE pPixelData = nullptr);
	static bool		SaveFile(HBITMAP hBitmap, const TCHAR* strSaveFile);
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
private:
	myBitmapManager();
public:
	~myBitmapManager();
};
#define g_BitmapMgr myBitmapManager::GetInstance()
