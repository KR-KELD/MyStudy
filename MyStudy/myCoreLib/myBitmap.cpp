#include "myBitmap.h"

HBITMAP myBitmap::LoadDIB(wstring strfileName)
{
	//파일 불러오기
	HANDLE hFile = CreateFile(strfileName.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	BITMAPFILEHEADER header;
	DWORD dwRead;
	BOOL bRet = ReadFile(hFile, &header, sizeof(BITMAPFILEHEADER),
		&dwRead, NULL);

	DWORD dwDataSize = header.bfOffBits -
		sizeof(BITMAPFILEHEADER);

	BITMAPINFO* pBitInfo =
		(BITMAPINFO*)malloc(dwDataSize);
	bRet = ReadFile(hFile, pBitInfo, dwDataSize, &dwRead, NULL);
	m_hBitmap = CreateDIBSection(g_hScreenDC,
		pBitInfo, DIB_RGB_COLORS,
		(LPVOID*)&m_pRaster, NULL, 0);

	ReadFile(hFile, m_pRaster,
		header.bfSize - header.bfOffBits,
		&dwRead, NULL);

	//1회용코드
	//if (strfileName.find(L"tank") != std::wstring::npos ||
	//	strfileName.find(L"artillery") != std::wstring::npos)

	if (pBitInfo->bmiHeader.biBitCount == 32)
	{
		for (int j = 0; j < pBitInfo->bmiHeader.biHeight; ++j)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[j * pBitInfo->bmiHeader.biWidth];
			for (int i = 0; i < pBitInfo->bmiHeader.biWidth; ++i)
			{
				if (pbSrcRGB[0] == 84 && pbSrcRGB[1] == 252 && pbSrcRGB[2] == 84)
				{
					pbSrcRGB[0] = 0;
					pbSrcRGB[1] = 0;
					pbSrcRGB[2] = 0;
					pbSrcRGB[3] = 125;
				}
				pbSrcRGB += 4;
			}
		}
	}

	free(pBitInfo);
	CloseHandle(hFile);
	return m_hBitmap;
}

bool myBitmap::CreateDIB(LONG iWidth, LONG iHeight, int iBitCount)
{
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = iBitCount;	//비트
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	m_hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi,
		DIB_RGB_COLORS, (LPVOID*)&m_pRaster, NULL, 0);

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	HBRUSH m_hbrBk = CreateSolidBrush(BKCOLOR);
	SelectObject(m_hMemDC, m_hbrBk);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;
}

bool myBitmap::Load(wstring strFileName)
{
	//비트맵 생성
	//m_hBitmap = (HBITMAP)LoadImage(
	//	g_hInstance, strFileName.c_str(),
	//	IMAGE_BITMAP, 0, 0,
	//	LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hBitmap = LoadDIB(strFileName.c_str());
	if (m_hBitmap == NULL)
	{
		return false;
	}
	//메인 DC를 임시DC에 복사
	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	//임시DC에 비트맵 추가
	SelectObject(m_hMemDC, m_hBitmap);

	HBRUSH m_hbrBk = CreateSolidBrush(BKCOLOR);
	SelectObject(m_hMemDC, m_hbrBk);

	//비트맵 정보 받아오기
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	return true;
}

bool myBitmap::DrawBit(RECT& rtDest, RECT& rtSrc, DWORD dwDrawType)
{
	//OffsetDC에 일정 영역만큼 임시DC를 복사
	//BitBlt = 소스를 1:1로 복사
	BitBlt(g_hOffScreenDC, rtDest.left, rtDest.top,
		rtDest.right, rtDest.bottom,
		m_hMemDC, rtSrc.left, rtSrc.top, dwDrawType);
	return true;
}

bool myBitmap::DrawStretch(RECT& rtDest, RECT& rtSrc, ReverseType _Reverse, DWORD dwDrawType, bool bInter)
{
	//StretchBlt = 소스의 비율을 조절가능 (뒤집기,확대축소)
	int iSave = 0;
	if(bInter)
		 iSave = SetStretchBltMode(g_hOffScreenDC, HALFTONE);
	if (_Reverse == REVERSE_LR)
	{
		StretchBlt(g_hOffScreenDC, rtDest.left + rtDest.right, rtDest.top,
			 -rtDest.right, rtDest.bottom, m_hMemDC, rtSrc.left,
			rtSrc.top, rtSrc.right, rtSrc.bottom, dwDrawType);
	}
	else if (_Reverse == REVERSE_UD)
	{
		StretchBlt(g_hOffScreenDC, rtDest.left, rtDest.top + rtDest.bottom,
			rtDest.right, -rtDest.bottom, m_hMemDC, rtSrc.left,
			rtSrc.top, rtSrc.right, rtSrc.bottom, dwDrawType);
	}
	else if (_Reverse == REVERSE_ALL)
	{
		StretchBlt(g_hOffScreenDC, rtDest.left + rtDest.right,
			rtDest.top + rtDest.bottom, -rtDest.right, -rtDest.bottom, 
			m_hMemDC, rtSrc.left, rtSrc.top, rtSrc.right, 
			rtSrc.bottom, dwDrawType);
	}
	else
	{
		StretchBlt(g_hOffScreenDC, rtDest.left, rtDest.top,
			rtDest.right, rtDest.bottom, m_hMemDC, rtSrc.left,
			rtSrc.top, rtSrc.right, rtSrc.bottom, dwDrawType);
	}
	if(bInter)
		SetStretchBltMode(g_hOffScreenDC, iSave);
	return true;
}

bool myBitmap::DrawTrans(RECT& rtDest, RECT& rtSrc, DWORD dwTransColor)
{
	//TransparrentBlt = 특정 색상을 그리지 않고 복사(확대축소가능)
	TransparentBlt(g_hOffScreenDC, rtDest.left, rtDest.top,
			rtDest.right, rtDest.bottom, m_hMemDC, rtSrc.left,
			rtSrc.top, rtSrc.right, rtSrc.bottom, dwTransColor);
	return true;
}

bool myBitmap::DrawTrans(LONG x, LONG y, DWORD dwTransColor)
{
	RECT dest = { x, y, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	RECT src = { 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };

	TransparentBlt(g_hOffScreenDC, dest.left, dest.top,
		dest.right, dest.bottom, m_hMemDC, src.left,
		src.top, src.right, src.bottom, dwTransColor);
	return true;
}

bool myBitmap::DrawAlphaBlend(RECT& rtDest, RECT& rtSrc, BYTE alpha)
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		rtDest.left, rtDest.top,
		rtDest.right, rtDest.bottom,
		m_hMemDC,
		rtSrc.left, rtSrc.top,
		rtSrc.right, rtSrc.bottom, m_BlendFunction);
	return true;
}

bool myBitmap::DrawAlphaBlend(RECT& rtDest, RECT& rtSrc, BLENDFUNCTION bf)
{
	AlphaBlend(g_hOffScreenDC,
		rtDest.left, rtDest.top,
		rtDest.right, rtDest.bottom,
		m_hMemDC,
		rtSrc.left, rtSrc.top,
		rtSrc.right, rtSrc.bottom, bf);
	return true;
}

bool myBitmap::DrawAlphaBlend(LONG x, LONG y, BYTE alpha)
{
	RECT dest = { x, y, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	RECT src = { 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };

	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;

	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		dest.left, dest.top,
		dest.right, dest.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, m_BlendFunction);
	return true;
}

bool myBitmap::DrawAlphaBlend(LONG x, LONG y, BYTE alpha, BLENDFUNCTION bf)
{
	RECT dest = { x, y, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	RECT src = { 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	//(cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		dest.left, dest.top,
		dest.right, dest.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, bf);
	return true;
}

bool myBitmap::Init()
{
	m_hBitmap = nullptr;
	m_hMemDC = nullptr;
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	return true;
}

bool myBitmap::Frame()
{
	return true;
}

bool myBitmap::Render()
{
	BitBlt(g_hOffScreenDC, 0, 0,
		g_rtClient.right, g_rtClient.bottom,
		m_hMemDC, 0, 0, SRCCOPY);
	return true;
}

bool myBitmap::Release()
{
	DeleteObject(m_hBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
}
