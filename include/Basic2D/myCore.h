#pragma once
#include "myWindow.h"
#include "myTimer.h"
#include "myInput.h"
#include "myGrapicsAPI.h"
#include "myDraw.h"
#include "mySoundManager.h"
class myCore : public myWindow
{
public:
	myGrapicsAPI		m_Graphics;
	bool				m_isGameRun;
public:
	virtual bool		Init() { return true; }
	virtual bool		Frame() { return true; }
	virtual bool		Render() { return true; }
	virtual bool		PreRender();
	virtual bool		PostRender();
	virtual bool		Release() { return true; }


private:
	bool				GameInit();
	bool				GameFrame();
	bool				GameRender();
	bool				GameRun();
	bool				GameRelease();
public:
	bool				Run();
};

