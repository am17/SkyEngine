#include "stdafx.h"
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(IIndexBufferImpl *aImpl)
	:_impl(aImpl)
{
}


IndexBuffer::~IndexBuffer()
{
	if (_impl)
	{
		delete _impl;
		_impl = nullptr;
	}
}

void IndexBuffer::bind(unsigned int offset)
{
	_impl->bind(offset);
}

unsigned int IndexBuffer::getIndexCount() const
{
	return _impl->getIndexCount();
}


