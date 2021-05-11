#pragma once



// myMapToolForm 폼 보기

class myMapToolForm : public CFormView
{
	DECLARE_DYNCREATE(myMapToolForm)

protected:
	myMapToolForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~myMapToolForm();
public:
	static myMapToolForm* CreateOnce(CWnd* pParent);
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_myMapToolForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CComboBox m_TileCount;
	CComboBox m_CellCount;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	int m_iCellSize;
	afx_msg void OnBnClickedButton2();

	CString m_strTexName;
	afx_msg void OnBnClickedTurret();
	afx_msg void OnBnClickedBarrel();
};


