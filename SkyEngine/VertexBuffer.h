#pragma once
#include "DeviceResourceImpl.h"
#include "common.h"

class VertexBuffer
{
public:
	VertexBuffer(IVertexBufferImpl *aImpl);
	~VertexBuffer();
	SKYENGINEDLL void bind(unsigned int startSlot = 0);
private:
	IVertexBufferImpl *_impl;
};

