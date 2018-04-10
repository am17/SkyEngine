#pragma once
#include "Resource.h"

class IndexBuffer : public Resource
{
public:
	IndexBuffer(unsigned int stride, unsigned int size, BUFFER_USAGE_FLAGS usage) :
		m_stride(stride),
		m_size(size),
		m_usage(usage)
	{}
	virtual ~IndexBuffer() {}
	unsigned int GetStride() const { return m_stride; }
	unsigned int GetSize() const { return m_size; }
	BUFFER_USAGE_FLAGS GetUsage() const { return m_usage; }
private:
	unsigned int m_stride;
	unsigned int m_size;
	BUFFER_USAGE_FLAGS m_usage;
};