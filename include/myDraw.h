#pragma once
#include "myStd.h"
struct myMsg
{
	RECT	rt;
	wstring	msg;
	myMsg(wstring _msg, RECT _rt)
	{
		msg = _msg;
		rt = _rt;
	}
	myMsg()
	{

	}
};
class myDraw : public SingleTon<myDraw>
{
private:
	HFONT			m_hFont;
	vector<myMsg>	m_TextList;
public:
	void	Push(myMsg msg);
	void	Push(wstring msg, int x = 0, int y = 0);
	void	Draw(POINT pos, wstring msg, DWORD color = RGB(255,0,0));
	void	Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	myDraw();
	virtual ~myDraw();
};
#define g_Draw myDraw::GetInstance()
