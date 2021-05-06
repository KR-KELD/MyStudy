#pragma once


// myPropertiesWnd

class myPropertiesWnd : public CDockablePane
{
	DECLARE_DYNAMIC(myPropertiesWnd)

public:
	CMFCPropertyGridCtrl m_wndPropList;

public:
	void InitPropList();

public:
	myPropertiesWnd();
	virtual ~myPropertiesWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


