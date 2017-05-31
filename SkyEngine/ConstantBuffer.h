#pragma once
#include "DeviceResourceImpl.h"
#include "ShaderTypes.h"
#include "common.h"

class ConstantBuffer
{
public:
	ConstantBuffer(IContantBufferImpl * aImpl);
	~ConstantBuffer();
	SKYENGINEDLL void bind(sky::EShaderType shaderType, unsigned int startSlot = 0);
	SKYENGINEDLL void update(void *aSrcData);
private:
	IContantBufferImpl *_impl;
};
