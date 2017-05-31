#pragma once
#include "DeviceResourceImpl.h"
#include "common.h"

class IndexBuffer
{
public:
	IndexBuffer(IIndexBufferImpl *aImpl);
	~IndexBuffer();
	SKYENGINEDLL void bind(unsigned int offset = 0);
	SKYENGINEDLL unsigned int getIndexCount() const;
private:
	IIndexBufferImpl *_impl;
};

