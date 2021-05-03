#pragma once
#include "myCore.h"

class Sample : public myCore
{

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};