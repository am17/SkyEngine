#pragma once
#include "Resource.h"

class VertexBuffer : public Resource
{
public:
	VertexBuffer(unsigned int size, BUFFER_USAGE_FLAGS usage) :
		m_size(size),
		m_usage(usage)
	{}
	virtual ~VertexBuffer(){}
	unsigned int GetSize() const { return m_size; }
	BUFFER_USAGE_FLAGS GetUsage() const { return m_usage; }
private:
	unsigned int m_size;
	BUFFER_USAGE_FLAGS m_usage;
};
