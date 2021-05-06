// myEffectToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myEffectToolForm.h"


// myEffectToolForm

IMPLEMENT_DYNCREATE(myEffectToolForm, CFormView)

myEffectToolForm::myEffectToolForm()
	: CFormView(IDD_myEffectToolForm)
{

}

myEffectToolForm::~myEffectToolForm()
{
}

myEffectToolForm * myEffectToolForm::CreateOnce(CWnd * pParent)
{
	myEffectToolForm* pForm = new myEffectToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100),
		pParent, 0, NULL);
	return pForm;
}

void myEffectToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(myEffectToolForm, CFormView)
END_MESSAGE_MAP()


// myEffectToolForm 진단

#ifdef _DEBUG
void myEffectToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void myEffectToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// myEffectToolForm 메시지 처리기
