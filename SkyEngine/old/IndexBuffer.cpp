#include "stdafx.h"
#include "IndexBuffer.h"
#include "globals.h"

namespace sky
{
	IndexBuffer::IndexBuffer(): 
		m_APIIndexBuffer(nullptr)
	{
	}


	IndexBuffer::~IndexBuffer()
	{
		if (m_APIIndexBuffer)
		{
			delete m_APIIndexBuffer;
			m_APIIndexBuffer = nullptr;
		}
	}

	IAPIBuffer* IndexBuffer::getAPIIndexBuffer() const
	{
		return m_APIIndexBuffer;
	}

	StaticIndexBuffer::StaticIndexBuffer()
		: rawData(nullptr),
		m_stride(0), 
		m_numIndices(0)
	{
	}

	StaticIndexBuffer::~StaticIndexBuffer()
	{
		if (rawData)
		{
			delete rawData;
			rawData = nullptr;
		}
	}

	void StaticIndexBuffer::init(const vector<unsigned int>& indices)
	{
		m_numIndices = indices.size();

		m_stride = sizeof(unsigned int);

		rawData = new unsigned int[m_numIndices];

		const void* rawVertexData = indices.data();

		memcpy_s(rawData, sizeof(unsigned int) * m_numIndices, rawVertexData, m_numIndices * sizeof(unsigned int));

		IDevice *device = global::getDevice();

		assert(device != nullptr);

		m_APIIndexBuffer = device->createIndexBuffer(rawData, m_numIndices, m_stride);
	}

	unsigned StaticIndexBuffer::getNumIndeces() const
	{
		return m_numIndices;
	}

	const void* StaticIndexBuffer::getRawData() const
	{
		return rawData;
	}

	unsigned StaticIndexBuffer::getStride() const
	{
		return m_stride;
	}
}
