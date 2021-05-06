// myOutputWnd.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myOutputWnd.h"


// myOutputWnd

IMPLEMENT_DYNAMIC(myOutputWnd, CDockablePane)

myOutputWnd::myOutputWnd()
{

}

myOutputWnd::~myOutputWnd()
{
}

BEGIN_MESSAGE_MAP(myOutputList, CListBox)
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP(myOutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// myOutputWnd 메시지 처리기



void myOutputWnd::PushBuild(wstring strData)
{
	m_wndOutputBuild.AddString(strData.c_str());
}

void myOutputWnd::PushDebug(wstring strData)
{
	m_wndOutputDebug.AddString(strData.c_str());
}

void myOutputWnd::PushFind(wstring strData)
{
	m_wndOutputFind.AddString(strData.c_str());
}

int myOutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("출력 탭 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 출력 창을 만듭니다.
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndTabs.AddTab(&m_wndOutputBuild, L"Build", 0);
	m_wndTabs.AddTab(&m_wndOutputDebug, L"Debug", 1);
	m_wndTabs.AddTab(&m_wndOutputFind, L"Find", 2);

	return TRUE;
}


void myOutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	m_wndTabs.SetWindowPos(nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int myOutputWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
