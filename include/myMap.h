#pragma once
#include "myGraphics.h"

struct myMapDesc
{
	int	iNumCols;
	int iNumRows;
	float fScaleHeight;
	float fCellDistance;
	T_STR szTexFile;
	T_STR szVS;
	T_STR szPS;
};

class myMap : public myGraphics
{
public:
	DEFINE_COMPONENT(myMap, myGraphics, false);
public:
	myMapDesc  m_MapDesc;
	UINT m_iNumRows;
	UINT m_iNumCols;
	UINT m_iNumCellCols;
	UINT m_iNumCellRows;
	UINT m_iNumVertices;
	UINT m_iNumFaces;
	float m_fCellDistance;
	std::vector<float> m_fHeightList;
public:
	bool			CreateMap(ID3D11DeviceContext*	pd3dContex, myMapDesc  desc);
	virtual bool	CalNormal();
	virtual float   Lerp(float fStart, float fEnd, float fTangent);
	virtual float	GetHeight(float fPosX, float fPosZ);
	virtual float   GetFaceHeight(UINT index);
	virtual float   GetHeightMap(int row, int col);
	virtual bool    CreateVertexData() override;
	virtual bool    CreateIndexData()override;
	virtual bool	Frame() override;
	virtual bool	PostRender();
public:
	myMap();
	virtual ~myMap();
};
