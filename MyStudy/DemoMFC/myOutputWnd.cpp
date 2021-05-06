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

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void myOutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

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
