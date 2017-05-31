#include "stdafx.h"
#include "ConstantBuffer.h"


ConstantBuffer::ConstantBuffer(IContantBufferImpl * aImpl)
	:_impl(aImpl)
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_impl)
	{
		delete _impl;
		_impl = nullptr;
	}
}

void ConstantBuffer::bind(sky::EShaderType shaderType, unsigned int startSlot)
{
	_impl->bind(shaderType, startSlot);
}

void ConstantBuffer::update(void *aSrcData)
{
	_impl->update(aSrcData);
}