#pragma once
#include "myFbxObj.h"

class myFbxLoader : public SingleTon<myFbxLoader>
{
	friend SingleTon<myFbxLoader>;
public:
	FbxManager*		m_pSDKManager;
public:
	bool		Init();
	bool		Release();
public:
	myFbxLoader();
	~myFbxLoader();
};
#define g_FbxLoader myFbxLoader::GetInstance()

