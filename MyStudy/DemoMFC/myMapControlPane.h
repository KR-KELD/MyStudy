#pragma once
#include "myMapControlForm.h"

// myMapControlPane

class myMapControlPane : public CDockablePane
{
	DECLARE_DYNAMIC(myMapControlPane)
public:
	myMapControlForm* m_wndForm;
public:
	myMapControlPane();
	virtual ~myMapControlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


