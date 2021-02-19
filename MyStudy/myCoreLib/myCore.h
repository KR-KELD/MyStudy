#pragma once
#include "myDevice.h"
#include "myTimer.h"
#include "myInput.h"
//#include "myGrapicsAPI.h"
#include "myDraw.h"
#include "mySoundManager.h"
#include "myDebugCamera.h"
#include "myObjManager.h"

class myCore : public myDevice
{
public:
	//myGrapicsAPI		m_Graphics;
	bool				m_isGameRun;
	myDebugCamera*		m_pDebugCamera;
	myCamera*		    m_pMainCamera;
	//myDebugCamera	    m_Camera;
	//myCamera*		    m_pMainCamera;
public:
	virtual bool		Init() { return true; }
	virtual bool		PreInit() { return true; }
	virtual bool		PostInit() { return true; }
	virtual bool		Frame() { return true; }
	virtual bool		PreFrame() { return true; }
	virtual bool		PostFrame() { return true; }
	virtual void		CameraFrame();
	virtual bool		Render() { return true; }
	virtual bool		PreRender();
	virtual bool		PostRender();
	virtual bool		Release() { return true; }
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource(UINT w, UINT h);

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

