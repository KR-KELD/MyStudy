//#include "myParsingScript.h"
//
//wstring myParsingScript::LoadFile(const TCHAR * szFileName)
//{
//	wifstream loadFile(szFileName);
//	wstring strData;
//	if (loadFile.is_open())
//	{
//		loadFile.seekg(0, ios::end);
//		streamoff size = loadFile.tellg();
//		strData.resize((unsigned int)size);
//		loadFile.seekg(0, ios::beg);
//		loadFile.read(&strData[0], size);
//		loadFile.close();
//	}
//	return strData;
//}
//
//myObject* myParsingScript::NewObj(myObjBase& base)
//{
//	myObject* objTemp = nullptr;
//	switch ((myObjType)base.iObjType)
//	{
//	case EFFECT: { objTemp = new myEffect; } break;
//	//case PROJECTILE: { objTemp = new myProjectile; } break;
//	//case PLAYER: { objTemp = new myPlayer; } break;
//	//case ENEMY:
//	//{
//	//	objTemp = new myNpc;
//	//	//myNpc* npcTemp = dynamic_cast<myNpc*>(objTemp);
//	//	//if (npcTemp != nullptr)
//	//	//{
//	//	//	npcTemp->FSM();
//	//	//}
//	//} break;
//	case UI: { objTemp = new myUI; } break;
//	//case BUTTON: { objTemp = new myButton; } break;
//	case BACKGROUND: { objTemp = new myUI; } break;
//	default: { objTemp = new myObject; } break;
//	}
//	return objTemp;
//}
//
//void myParsingScript::ObjPoolParsing(wstring strData)
//{
//	wstringstream ss(strData);
//	wstring strTemp;
//
//	int iCount = 0;
//	int iPoolSize = 0;
//	ss >> iCount;
//	myObjectInfo* infoTemp = nullptr;
//
//	while (iCount != 0)
//	{
//		ss >> strTemp;
//		if (strTemp == L"Obj")
//		{
//			infoTemp = new myObjectInfo;
//			ss >> infoTemp->iObjType;
//		}
//		if (strTemp == L"PoolSize")
//		{
//			ss >> iPoolSize;
//		}
//		else if (strTemp == L"Name")
//		{
//			ss >> infoTemp->strName;
//		}
//		else if (strTemp == L"ObjName")
//		{
//			ss >> infoTemp->strObjName;
//		}
//		else if (strTemp == L"SpriteName")
//		{
//			ss >> infoTemp->strSpriteName;
//			if (infoTemp->strSpriteName == L"null")
//			{
//				infoTemp->strSpriteName.clear();
//				infoTemp->bSprite = false;
//			}
//			else
//			{
//				infoTemp->iMaxCount =
//					(int)g_ObjMgr.GetPtr(infoTemp->strObjName)->
//					GetRtSpriteList()[infoTemp->strSpriteName].size();
//				ss >> strTemp;
//				if (strTemp == L"Loop")
//				{
//					ss >> strTemp;
//					if (strTemp == L"true")
//						infoTemp->bLoop = true;
//					else
//						infoTemp->bLoop = false;
//				}
//				ss >> strTemp;
//				if (strTemp == L"Interval")
//				{
//					ss >> infoTemp->fInterval;
//				}
//				ss >> strTemp;
//				if (strTemp == L"Round")
//				{
//					ss >> strTemp;
//					if (strTemp == L"true")
//						infoTemp->bRound = true;
//					else
//						infoTemp->bRound = false;
//				}
//				ss >> strTemp;
//				if (strTemp == L"IsEffect")
//				{
//					ss >> strTemp;
//					if (strTemp == L"true")
//						infoTemp->bSprite = true;
//					else
//						infoTemp->bSprite = false;
//				}
//			}
//		}
//		else if (strTemp == L"Collider")
//		{
//			ss >> infoTemp->iCollWidth;
//			ss >> infoTemp->iCollHeight;
//		}
//		else if (strTemp == L"Dead")
//		{
//			ss >> strTemp;
//			if (strTemp == L"true")
//				infoTemp->bDead = true;
//			else
//				infoTemp->bDead = false;
//		}
//		else if (strTemp == L"Speed")
//		{
//			ss >> infoTemp->fSpeed;
//		}
//		else if (strTemp == L"LifeTime")
//		{
//			ss >> infoTemp->fLifeTime;
//		}
//		else if (strTemp == L"End")
//		{
//			vector<myObjectInfo*> vecTemp;
//			vecTemp.resize(iPoolSize);
//			myObjectInfo* copyTemp = nullptr;
//			vecTemp[0] = infoTemp;
//			for (int i = 1; i < vecTemp.size(); i++)
//			{
//				copyTemp = new myObjectInfo(infoTemp);
//				vecTemp[i] = copyTemp;
//			}
//			if (infoTemp->iObjType == 1)
//			{
//				g_ObjPoolMgr.AddEffectPool(infoTemp->strName, vecTemp);
//			}
//			else
//			{
//				g_ObjPoolMgr.AddProjectilePool(infoTemp->strName, vecTemp);
//			}
//			strTemp.clear();
//			iCount -= 1;
//			infoTemp = nullptr;
//		}
//	}
//}
//
//void myParsingScript::ObjBaseParsing(wstring strData)
//{
//	wstringstream ss(strData);
//	wstring strTemp;
//	wstring strName;
//	int iCount = 0;
//	ss >> iCount;
//	myObjBase objBaseTemp;
//	while (iCount != 0)
//	{
//		ss >> strTemp;
//		if (strTemp == L"Obj")
//		{
//			objBaseTemp.Reset();
//			ss >> objBaseTemp.iObjType;
//		}
//		else if (strTemp == L"Name")
//		{
//			ss >> strName;
//			objBaseTemp.strName = strName;
//		}
//		else if (strTemp == L"Image")
//		{
//			ss >> objBaseTemp.strImageName;
//			if (objBaseTemp.strImageName == L"null")
//				objBaseTemp.strImageName.clear();
//		}
//		else if (strTemp == L"Mask")
//		{
//			ss >> objBaseTemp.strMaskName;
//			if (objBaseTemp.strMaskName == L"null")
//				objBaseTemp.strMaskName.clear();
//		}
//		else if (strTemp == L"Alpha")
//		{
//			ss >> strTemp;
//			if (strTemp == L"true")
//				objBaseTemp.bAlpha = true;
//			else
//				objBaseTemp.bAlpha = false;
//			ss >> objBaseTemp.iAlphaValue;
//		}
//		else if (strTemp == L"Trans")
//		{
//			ss >> strTemp;
//			if (strTemp == L"true")
//				objBaseTemp.bTrans = true;
//			else
//				objBaseTemp.bTrans = false;
//		}
//		else if (strTemp == L"TransColor")
//		{
//			//ss >> objBaseTemp.dwTransColor;
//			int r, g, b = 0;
//			ss >> r; ss >> g; ss >> b;
//			objBaseTemp.dwTransColor = RGB(r, g, b);
//		}
//		else if (strTemp == L"Sprite")
//		{
//			int iSize = 0;
//			ss >> iSize;
//			for (int i = 0; i < iSize; i++)
//			{
//				ss >> strTemp;
//				if (strTemp == L"SpriteName")
//				{
//					ss >> strTemp;
//					int iSize2 = 0;
//					int iIndex = 0;
//					RECT rtTemp;
//					RECT_ARRAY rtArrayTemp;
//					ss >> iSize2;
//					rtArrayTemp.resize(iSize2);
//					for (int j = 0; j < iSize2; j++)
//					{
//						ss >> iIndex;
//						ss >> rtTemp.left;
//						ss >> rtTemp.top;
//						ss >> rtTemp.right;
//						ss >> rtTemp.bottom;
//						rtArrayTemp[iIndex] = rtTemp;
//					}
//					objBaseTemp.mapRtSpriteList.insert(make_pair(strTemp, rtArrayTemp));
//				}
//			}
//			ss >> strTemp;
//			if (strTemp == L"SpriteEnd")
//			{
//				g_ObjMgr.GetPtr(strName)->SetRtSpriteList(objBaseTemp.mapRtSpriteList);
//				strName.clear();
//			}
//		}
//		else if (strTemp == L"End")
//		{
//			myObject* objTemp = NewObj(objBaseTemp);
//
//			objTemp->Init();
//			objTemp->SetObjBase(objBaseTemp);
//			g_ObjMgr.Add(objBaseTemp.strName, objTemp);
//			strTemp.clear();
//			iCount -= 1;
//		}
//	}
//}
//
//void myParsingScript::ObjInfoParsing(wstring strData, vector<myObjectInfo*>* vecInfo)
//{
//	wstringstream ss(strData);
//	wstring strTemp;
//
//	int iCount = 0;
//	ss >> iCount;
//	myObjectInfo* infoTemp = nullptr;
//
//	while (iCount != 0)
//	{
//		ss >> strTemp;
//		if (strTemp == L"Obj")
//		{
//			infoTemp = new myObjectInfo;
//			ss >> infoTemp->iObjType;
//		}
//		else if (strTemp == L"TeamNum")
//		{
//			ss >> infoTemp->iTeamNum;
//		}
//		else if (strTemp == L"Name")
//		{
//			ss >> infoTemp->strName;
//		}
//		else if (strTemp == L"ObjName")
//		{
//			ss >> infoTemp->strObjName;
//		}
//		else if (strTemp == L"SpriteName")
//		{
//			ss >> infoTemp->strSpriteName;
//			if (infoTemp->strSpriteName == L"null")
//			{
//				infoTemp->strSpriteName.clear();
//				infoTemp->bSprite = false;
//			}
//			else
//			{
//				infoTemp->iMaxCount =
//					(int)g_ObjMgr.GetPtr(infoTemp->strObjName)->
//					GetRtSpriteList()[infoTemp->strSpriteName].size();
//				ss >> strTemp;
//				if (strTemp == L"Loop")
//				{
//					ss >> strTemp;
//					if (strTemp == L"true")
//						infoTemp->bLoop = true;
//					else
//						infoTemp->bLoop = false;
//				}
//				ss >> strTemp;
//				if (strTemp == L"Interval")
//				{
//					ss >> infoTemp->fInterval;
//				}
//				ss >> strTemp;
//				if (strTemp == L"Round")
//				{
//					ss >> strTemp;
//					if (strTemp == L"true")
//						infoTemp->bRound = true;
//					else
//						infoTemp->bRound = false;
//				}
//				ss >> strTemp;
//				if (strTemp == L"IsEffect")
//				{
//					ss >> strTemp;
//					if (strTemp == L"true")
//						infoTemp->bSprite = true;
//					else
//						infoTemp->bSprite = false;
//				}
//			}
//		}
//		else if (strTemp == L"Pos")
//		{
//			ss >> infoTemp->ptPos.x;
//			ss >> infoTemp->ptPos.y;
//		}
//		//else if (strTemp == L"Dir")
//		//{
//		//	ss >> infoTemp->ptDir.x;
//		//	ss >> infoTemp->ptDir.y;
//		//}
//		else if (strTemp == L"Collider")
//		{
//			ss >> infoTemp->iCollWidth;
//			ss >> infoTemp->iCollHeight;
//		}
//		else if (strTemp == L"HP")
//		{
//			ss >> infoTemp->iMaxHP;
//			infoTemp->iCurrentHP = infoTemp->iMaxHP;
//		}
//		else if (strTemp == L"Damage")
//		{
//			ss >> infoTemp->iDamage;
//		}
//		else if (strTemp == L"Dead")
//		{
//			ss >> strTemp;
//			if (strTemp == L"true")
//				infoTemp->bDead = true;
//			else
//				infoTemp->bDead = false;
//		}
//		else if (strTemp == L"Speed")
//		{
//			ss >> infoTemp->fSpeed;
//		}
//		else if (strTemp == L"LookRange")
//		{
//			ss >> infoTemp->fLookRange;
//		}
//		else if (strTemp == L"AttRange")
//		{
//			ss >> infoTemp->fAttRange;
//		}
//		else if (strTemp == L"AttDelay")
//		{
//		ss >> infoTemp->fAttDelay;
//		}
//		else if (strTemp == L"LifeTime")
//		{
//			ss >> infoTemp->fLifeTime;
//		}
//		else if (strTemp == L"End")
//		{
//			(*vecInfo).push_back(infoTemp);
//			strTemp.clear();
//			iCount -= 1;
//			infoTemp = nullptr;
//		}
//	}
//}
//
//void myParsingScript::SaveObjInfoFile(const TCHAR* szFileName, vector<myObjectInfo*>* vecAtt)
//{
//	wstring dataTemp = L"";
//	size_t iSize = vecAtt->size();
//	dataTemp += to_wstring(iSize);
//	dataTemp += L"\n\n";
//	for (myObjectInfo* objInfo : *vecAtt)
//	{
//		dataTemp += L"Name ";
//		StrCheck(&objInfo->strName);
//		dataTemp += objInfo->strName;
//		dataTemp += L" TeamNum ";
//		dataTemp += to_wstring(objInfo->iTeamNum);
//		dataTemp += L" ObjName ";
//		StrCheck(&objInfo->strObjName);
//		dataTemp += objInfo->strObjName;
//		dataTemp += L" SpriteName ";
//		StrCheck(&objInfo->strSpriteName);
//		if (objInfo->strSpriteName != L"null")
//		{
//			dataTemp += objInfo->strSpriteName;
//			dataTemp += L" Loop ";
//			dataTemp += BoolToStr(objInfo->bLoop);
//			dataTemp += L" Interval ";
//			dataTemp += to_wstring(objInfo->fInterval);
//			dataTemp += L" Round ";
//			dataTemp += BoolToStr(objInfo->bRound);
//			dataTemp += L" IsEffect ";
//			dataTemp += BoolToStr(objInfo->bSprite);
//		}
//		dataTemp += L" Pos ";
//		dataTemp += to_wstring(objInfo->ptPos.x);
//		dataTemp += L" ";
//		dataTemp += to_wstring(objInfo->ptPos.y);
//		//dataTemp += L" Dir ";
//		//dataTemp += to_wstring(objInfo->ptDir.x);
//		//dataTemp += L" ";
//		//dataTemp += to_wstring(objInfo->ptDir.y);
//		dataTemp += L" Dest ";
//		dataTemp += RectToStr(objInfo->rtDest);
//		dataTemp += L" Src ";
//		dataTemp += RectToStr(objInfo->rtSrc);
//		dataTemp += L" Collider ";
//		dataTemp += to_wstring(objInfo->iCollWidth);
//		dataTemp += L" ";
//		dataTemp += to_wstring(objInfo->iCollHeight);
//		dataTemp += L" HP ";
//		dataTemp += to_wstring(objInfo->iMaxHP);
//		dataTemp += L" Damage ";
//		dataTemp += to_wstring(objInfo->iDamage);
//		dataTemp += L" Dead ";
//		dataTemp += BoolToStr(objInfo->bDead);
//		dataTemp += L" Speed ";
//		dataTemp += to_wstring(objInfo->fSpeed);
//		dataTemp += L" LookRange ";
//		dataTemp += to_wstring(objInfo->fLookRange);
//		dataTemp += L" AttRange ";
//		dataTemp += to_wstring(objInfo->fAttRange);
//		dataTemp += L" AttDelay ";
//		dataTemp += to_wstring(objInfo->fAttDelay);
//		dataTemp += L" LifeTime ";
//		dataTemp += to_wstring(objInfo->fLifeTime);
//		dataTemp += L" End \n";
//	}
//
//	wofstream wFile(szFileName);
//	if (wFile.is_open())
//		wFile << dataTemp;
//	wFile.close();
//}
//
////void myParsingScript::SaveObjBaseFile(const TCHAR * szFileName, vector<myObjBase>* vecObj)
////{
////	wstring dataTemp = L"";
////	size_t iSize = vecObj->size();
////	dataTemp += to_wstring(iSize);
////	dataTemp += L"\n\n";
////	for (myObjBase obj : *vecObj)
////	{
////		dataTemp += L"Obj ";
////		dataTemp += to_wstring(obj.iObjType);
////		dataTemp += L"Name ";
////		dataTemp += obj.strName;
////		dataTemp += L" Image ";
////		StrCheck(&obj.strImageName);
////		dataTemp += obj.strImageName;
////		dataTemp += L" Mask ";
////		StrCheck(&obj.strMaskName);
////		dataTemp += obj.strMaskName;
////		dataTemp += L" Alpha ";
////		dataTemp += BoolToStr(obj.bAlpha);
////		dataTemp += L" ";
////		dataTemp += to_wstring(obj.iAlphaValue);
////		dataTemp += L" Trans ";
////		dataTemp += BoolToStr(obj.bTrans);
////		dataTemp += L" TransColor ";
////		dataTemp += to_wstring(obj.dwTransColor);
////		dataTemp += L" End \n";
////	}
////	wofstream wFile(szFileName);
////	if (wFile.is_open())
////		wFile << dataTemp;
////	wFile.close();
////}
//
//wstring myParsingScript::BoolToStr(bool b)
//{
//	wstring strTemp;
//	if (b)
//		strTemp = L"true";
//	else
//		strTemp = L"false";
//	return strTemp;
//}
//
//void myParsingScript::StrCheck(wstring* str)
//{
//	if (str->empty())
//		(*str) = L"null";
//}
//
//wstring myParsingScript::RectToStr(RECT & rt)
//{
//	wstring strTemp;
//	strTemp += to_wstring(rt.left);
//	strTemp += L" ";
//	strTemp += to_wstring(rt.top);
//	strTemp += L" ";
//	strTemp += to_wstring(rt.right);
//	strTemp += L" ";
//	strTemp += to_wstring(rt.bottom);
//	return strTemp;
//}
//
//bool myParsingScript::Init()
//{
//	return true;
//}
//
//bool myParsingScript::Frame()
//{
//	return true;
//}
//
//bool myParsingScript::Render()
//{
//	return true;
//}
//
//bool myParsingScript::Release()
//{
//	return true;
//}
//
//myParsingScript::myParsingScript()
//{
//}
//
//myParsingScript::~myParsingScript()
//{
//}
