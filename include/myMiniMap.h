#pragma once
#include "myDxRT.h"
#include "myShape.h"
class myMiniMap : public myShapePlane
{
public:
	DEFINE_COMPONENT(myMiniMap, myShapePlane, false);
public:
	myDxRT			m_DxRT;
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
	virtual bool	Create(T_STR szVS, T_STR szPS, T_STR szTex);
	virtual bool	Begin();
	virtual bool	End();
	virtual bool	Release() override;
	virtual bool	PostRender() override;
	virtual bool	Draw();
};
