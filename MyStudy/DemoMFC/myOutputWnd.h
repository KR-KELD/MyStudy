#pragma once


// myOutputWnd
class myOutputList : public CListBox
{
public:
	myOutputList() {};
	virtual ~myOutputList() {};
	DECLARE_MESSAGE_MAP();

};

class myOutputWnd : public CDockablePane
{
	DECLARE_DYNAMIC(myOutputWnd)
public:
	CMFCTabCtrl		m_wndTabs;
	myOutputList	m_wndOutputBuild;
	myOutputList	m_wndOutputDebug;
	myOutputList	m_wndOutputFind;
public:
	void PushBuild(wstring strData);
	void PushDebug(wstring strData);
	void PushFind(wstring strData);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
public:
	myOutputWnd();
	virtual ~myOutputWnd();

protected:
	DECLARE_MESSAGE_MAP()
};


