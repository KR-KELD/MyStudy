#pragma once
#include "myFbxObj.h"

class myFbxLoader : public SingleTon<myFbxLoader>
{
	friend SingleTon<myFbxLoader>;
public:

public:
	myFbxLoader();
	~myFbxLoader();
};
#define g_FbxLoader myFbxLoader::GetInstance()

