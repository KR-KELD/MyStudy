#include "myCore.h"

bool myCore::GameInit()
{
	m_isGameRun = true;
	g_Timer.Init();
	g_Input.Init();
	g_Draw.Init();
	m_Graphics.Init();
	g_SoundMgr.Init();
	Init();
	return true;
}

bool myCore::GameFrame()
{
	g_Timer.Frame();
	g_Input.Frame();
	m_Graphics.Frame();
	g_SoundMgr.Frame();
	Frame();
	g_Draw.Frame();
	return true;
}

bool myCore::GameRender()
{
	PreRender();
	Render();
	PostRender();
	return true;
}

bool myCore::PreRender()
{
	m_Graphics.PreRender();
	return true;
}

bool myCore::PostRender()
{
	g_Timer.Render();
	g_Input.Render();
	g_SoundMgr.Render();
	g_Draw.Render();
	g_Draw.Draw(0, 0, g_Timer.m_szBuffer);
	m_Graphics.PostRender();
	return true;
}

bool myCore::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}

bool myCore::Run()
{
	GameInit();
	while (m_isGameRun)
	{
		if (MsgProcess() == true)
		{
			GameRun();
		}
		else
		{
			m_isGameRun = false;
		}
	}
	GameRelease();
	return true;
}

bool myCore::GameRelease()
{
	Release();
	g_Draw.Release();
	g_Timer.Release();
	g_Input.Release();
	g_SoundMgr.Release();
	m_Graphics.Release();
	return true;
}