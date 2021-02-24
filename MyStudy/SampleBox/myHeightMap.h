#pragma once
#include "myMap.h"
class myHeightMap : public myMap
{
public:
	DEFINE_COMPONENT(myHeightMap, myMap, false);
public:
	float   GetHeight(UINT index) override;
	bool	CreateHeightMap(ID3D11DeviceContext* pContext, const TCHAR* pszFileName);
};

