#pragma once



// myEffectToolForm 폼 보기

class myEffectToolForm : public CFormView
{
	DECLARE_DYNCREATE(myEffectToolForm)

protected:
	myEffectToolForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~myEffectToolForm();
public:
	static myEffectToolForm* CreateOnce(CWnd* pParent);
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_myEffectToolForm };
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
};


