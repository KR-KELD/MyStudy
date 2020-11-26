#pragma once
#include "myStd.h"
#define MYGAME_START int APIENTRY wWinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance,LPWSTR    lpCmdLine,	int		nCmdShow)	{
#define MYGAME_RUN 	Sample demo;if (demo.SetWindow(hInstance)){	demo.Run();}return 1;}
#define GAMERUN MYGAME_START;MYGAME_RUN;