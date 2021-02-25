#pragma once
#include "myDevice.h"
#include "myTimer.h"
#include "myInput.h"
//#include "myGrapicsAPI.h"
#include "myDraw.h"
#include "mySoundManager.h"
#include "myDebugCamera.h"
#include "myObjManager.h"
#include "myCameraManager.h"
#include "myShape.h"
#include "myController.h"

class myCore : public myDevice
{
public:
	//myGrapicsAPI		m_Graphics;
	bool				m_isGameRun;
	myShapeLine*		m_pBasisLine;
public:
	virtual bool		Init() { return true; }
	virtual bool		PreInit() { return true; }
	virtual bool		PostInit() { return true; }
	virtual bool		Frame() { return true; }
	virtual bool		PreFrame() { return true; }
	virtual bool		PostFrame() { return true; }
	virtual bool		Render() { return true; }
	virtual bool		PreRender();
	virtual bool		PostRender();
	virtual bool		Release() { return true; }
	virtual HRESULT		DeleteDXResource();
	virtual HRESULT		CreateDXResource(UINT w, UINT h);
private:
	bool				GameInit();
	bool				GameFrame();
	bool				GameRender();
	bool				GameRun();
	bool				GameRelease();
public:
	bool				Run();
public:
	myCore();
	virtual ~myCore();
};

