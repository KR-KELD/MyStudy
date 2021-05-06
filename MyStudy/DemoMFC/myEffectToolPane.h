#pragma once
#include "myEffectToolForm.h"

// myEffectToolPane

class myEffectToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(myEffectToolPane)
public:
	myEffectToolForm* m_wndForm;
public:
	myEffectToolPane();
	virtual ~myEffectToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


