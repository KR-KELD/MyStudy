#pragma once


// myMapDlg 대화 상자

class myMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(myMapDlg)

public:
	myMapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~myMapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_myMapDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_TileCount;
	CComboBox m_CellCount;
	CEdit m_CellSize;
	afx_msg void OnBnClickedOk();
};
