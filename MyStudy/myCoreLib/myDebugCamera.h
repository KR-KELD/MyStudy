#pragma once
#include "myCamera.h"
class myDebugCamera : public myCamera
{

public:
	void Update(Vector4 data);
	bool Init();
	bool Frame();
public:
	myDebugCamera();
	virtual ~myDebugCamera();
};

