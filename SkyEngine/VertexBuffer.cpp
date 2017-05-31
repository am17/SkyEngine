#include "stdafx.h"
#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(IVertexBufferImpl *aImpl)
	:_impl(aImpl)
{
}


VertexBuffer::~VertexBuffer()
{
	if (_impl)
	{
		delete _impl;
		_impl = nullptr;
	}
}

void VertexBuffer::bind(unsigned int startSlot)
{
	_impl->bind(startSlot);
}
