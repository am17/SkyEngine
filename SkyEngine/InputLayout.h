#pragma once
#include "DeviceResourceImpl.h"
#include "common.h"

class InputLayout
{
public:
	InputLayout(IInputLayoutImpl *aImpl);
	~InputLayout();
	SKYENGINEDLL void bind();
private:
	IInputLayoutImpl *_pImpl;
};

