// myPropertiesWnd.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myPropertiesWnd.h"


// myPropertiesWnd

IMPLEMENT_DYNAMIC(myPropertiesWnd, CDockablePane)

void myPropertiesWnd::InitPropList()
{
	CMFCPropertyGridProperty* pGroup1 = nullptr;
	pGroup1 = new CMFCPropertyGridProperty(L"aaaa");
	CMFCPropertyGridProperty* pGroup2 = nullptr;
	pGroup2 = new CMFCPropertyGridProperty(L"bbbb");

	CMFCPropertyGridProperty* pData = new CMFCPropertyGridProperty(L"a1",L"Dlg");
	pData->AddOption(L"00001");
	pData->AddOption(L"00002");
	pData->AddOption(L"00003");
	pData->AddOption(L"00004");
	pData->AddOption(L"00005");
	pData->AllowEdit(FALSE);
	pGroup1->AddSubItem(pData);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(L"크기",90001l,L"크기를 지정하시오");
	pSize->EnableSpinControl(TRUE, 0, 100);
	pGroup1->AddSubItem(pSize);

	CMFCPropertyGridColorProperty* pColor = new CMFCPropertyGridColorProperty(
		L"컬러", RGB(100,100,100),nullptr, L"색상을 지정하시오");
	pColor->EnableOtherButton(L"기타");
	pColor->EnableAutomaticButton(L"기본값",GetSysColor(COLOR_3DFACE));
	pGroup1->AddSubItem(pColor);

	m_wndPropList.AddProperty(pGroup1);
	m_wndPropList.AddProperty(pGroup2);
}

myPropertiesWnd::myPropertiesWnd()
{

}

myPropertiesWnd::~myPropertiesWnd()
{
}


BEGIN_MESSAGE_MAP(myPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()



// myPropertiesWnd 메시지 처리기




int myPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect rt;
	rt.SetRectEmpty();
	if (!m_wndPropList.Create(WS_VISIBLE |WS_CHILD, rt, this, 3333))
	{
		return -1;
	}
	InitPropList();
	return 0;
}


void myPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	CRect rt;
	GetClientRect(rt);

	if (m_wndPropList)
	{
		m_wndPropList.SetWindowPos(NULL, rt.left, rt.top, rt.right, rt.bottom,
			SWP_NOZORDER |SWP_NOACTIVATE);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int myPropertiesWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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

LRESULT myPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	COleVariant rVariant = pProp->GetValue();
	VARIANT var = rVariant.Detach();
	return 0;
}

