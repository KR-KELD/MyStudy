#pragma once
#include "myCamera.h"
class myDebugCamera : public myCamera
{
public:
	DEFINE_COMPONENT(myDebugCamera, myCamera, false)
public:
	void Update(Vector4 data);
	bool Init();
	bool Frame();
public:
	myDebugCamera();
	virtual ~myDebugCamera();
};

