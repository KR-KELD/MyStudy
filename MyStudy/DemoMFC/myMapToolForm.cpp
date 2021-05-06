// myMapToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoMFC.h"
#include "myMapToolForm.h"


// myMapToolForm

IMPLEMENT_DYNCREATE(myMapToolForm, CFormView)

myMapToolForm::myMapToolForm()
	: CFormView(IDD_myMapToolForm)
{

}

myMapToolForm::~myMapToolForm()
{
}

myMapToolForm * myMapToolForm::CreateOnce(CWnd * pParent)
{
	myMapToolForm* pForm = new myMapToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100),
		pParent, 0, NULL);
	return pForm;
}

void myMapToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(myMapToolForm, CFormView)
END_MESSAGE_MAP()


// myMapToolForm 진단

#ifdef _DEBUG
void myMapToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void myMapToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// myMapToolForm 메시지 처리기
