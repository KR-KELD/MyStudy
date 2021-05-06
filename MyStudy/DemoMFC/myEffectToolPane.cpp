// myEffectToolPane.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myEffectToolPane.h"


// myEffectToolPane

IMPLEMENT_DYNAMIC(myEffectToolPane, CDockablePane)

myEffectToolPane::myEffectToolPane()
{

}

myEffectToolPane::~myEffectToolPane()
{
}


BEGIN_MESSAGE_MAP(myEffectToolPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// myEffectToolPane 메시지 처리기


int myEffectToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_wndForm = myEffectToolForm::CreateOnce(this);
	return 0;
}


void myEffectToolPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int myEffectToolPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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
