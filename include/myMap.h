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
//정점마다 가지고있는 인접 페이스노말테이블
struct myNormalLookupTable
{
	int  index[6];
	myNormalLookupTable()
	{
		index[0] = -1;
		index[1] = -1;
		index[2] = -1;
		index[3] = -1;
		index[4] = -1;
		index[5] = -1;
	}
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
	std::vector<Vector3> m_FaceNormals;
	std::vector<myNormalLookupTable> m_LookupTabel;
	Vector3 ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	void CalcFaceNormals();
	void GetVertexNormal();
	void InitFaceNormals();
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();
public:
	bool			CreateMap(myMapDesc  desc);
	//virtual bool	CalNormal();
	virtual float   Lerp(float fStart, float fEnd, float fTangent);
	virtual float	GetHeight(float fPosX, float fPosZ);
	virtual float   GetFaceHeight(UINT index);
	virtual float   GetHeightMap(int row, int col);
	virtual bool    CreateVertexData() override;
	virtual bool    CreateIndexData()override;
	virtual bool	Frame() override;
	virtual bool	Draw() override;
public:
	myMap();
	virtual ~myMap();
};
