#pragma once
#include "myMapToolForm.h"

// myMapToolPane

class myMapToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(myMapToolPane)
public:
	myMapToolForm* m_wndForm;
public:
	myMapToolPane();
	virtual ~myMapToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


