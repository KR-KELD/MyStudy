#pragma once
#include "myFbxObj.h"

class myFbxLoader : public SingleTon<myFbxLoader>
{
	friend SingleTon<myFbxLoader>;
public:
	FbxManager*		m_pSDKManager;
	std::map<string, myFbxObj*>				m_List;
	std::map<string, myFbxObj*>::iterator	m_iter;
	std::string								m_szDefaultPath;
public:
	myFbxObj*    Load(const char* szFileName);
	myFbxObj*    GetPtr(string szFileName);

	bool		Init();
	bool		Release();
public:
	myFbxLoader();
	~myFbxLoader();
};
#define g_FbxLoader myFbxLoader::GetInstance()

