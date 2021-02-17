#pragma once
#include "myComponent.h"
class myTransform : public myComponent
{
public:
	Vector3		m_vPos;
	Quaternion	m_qRot;
	Vector3		m_vScale;

public:
	Vector3		m_vLook;
	Vector3		m_vUp;
	Vector3		m_vRight;
public:
	T_STR		m_strTag;
public:
	void ComputeMatWorld();
public:
	virtual bool	Init() override;
	virtual bool	PreFrame() override;
	virtual bool	Frame() override;
	virtual bool	PostFrame() override;
	virtual bool	PreRender() override;
	virtual bool	Render() override;
	virtual bool	PostRender() override;
	virtual void	Update() override;
	virtual void	Reset() override;
	virtual bool	Action() override;
	virtual bool	Release() override;
};

