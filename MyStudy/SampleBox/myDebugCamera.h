#pragma once
#include "myCamera.h"
class myDebugCamera : public myCamera
{
	Vector4 m_vDirValue;
public:
	void Update(Vector4 data);
	bool Frame();
public:
	myDebugCamera();
	virtual ~myDebugCamera();
};

