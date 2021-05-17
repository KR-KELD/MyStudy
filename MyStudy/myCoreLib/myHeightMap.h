#pragma once
#include "myMap.h"
class myHeightMap : public myMap
{
public:
	DEFINE_COMPONENT(myHeightMap, myMap, false);
public:
	float   GetFaceHeight(UINT index) override;
	bool	CreateHeightMap(const TCHAR* pszFileName, myMapDesc& mapDesc);
};

