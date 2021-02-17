//#pragma once
////#include "myPlayer.h"
////#include "myNpc.h"
////#include "myButton.h"
//#include "myUI.h"
//#include "myEffect.h"
//#include "myObjManager.h"
//
//enum myObjType
//{
//	OBJECT = 0,
//	EFFECT,
//	PROJECTILE,
//	PLAYER = 100,
//	NPC = 200,
//	ENEMY,
//	UI = 300,
//	BUTTON,
//	PROGRESSBAR,
//	BACKGROUND = 400,
//	TILE = 500
//};
//
//class myParsingScript
//{
//
//public:
//	wstring					LoadFile(const TCHAR* szFileName);
//	virtual myObject*		NewObj(myObjBase& base);
//	void					ObjPoolParsing(wstring strData);
//	void					ObjBaseParsing(wstring strData);
//	void					ObjInfoParsing(wstring strData, vector<myObjectInfo*>* vecAtt);
//	void					SaveObjInfoFile(const TCHAR* szFileName, vector<myObjectInfo*>* vecInfo);
//	//void					SaveObjBaseFile(const TCHAR* szFileName, vector<myObjBase>* vecObj);
//	wstring					BoolToStr(bool b);
//	void					StrCheck(wstring* str);
//	wstring					RectToStr(RECT& rt);
//public:
//	virtual bool			Init();
//	virtual bool			Frame();
//	virtual bool			Render();
//	virtual bool			Release();
//public:
//	myParsingScript();
//	~myParsingScript();
//};
//

