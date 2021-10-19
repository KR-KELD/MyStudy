#pragma once



// myMapControlForm 폼 보기

class myMapControlForm : public CFormView
{
	DECLARE_DYNCREATE(myMapControlForm)

protected:
	myMapControlForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~myMapControlForm();
public:
	static myMapControlForm* CreateOnce(CWnd* pParent);
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_myMapControlForm };
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
	afx_msg void OnBnClickedTerrainUp();
	afx_msg void OnBnClickedTerrainDown();
	afx_msg void OnBnClickedTerrainFlat();
	float m_fRadius;
	float m_fSpeed;
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedSplat1();
	afx_msg void OnBnClickedSplat2();
	afx_msg void OnBnClickedSplat3();
	afx_msg void OnBnClickedSplat4();
	virtual void OnInitialUpdate();
	CStatic m_PictureControl;
	afx_msg void OnBnClickedBarrel();
	afx_msg void OnBnClickedTurret();
	afx_msg void OnBnClickedMove();
	afx_msg void OnBnClickedTurn();
	afx_msg void OnBnClickedScale();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSplatErase();
};


