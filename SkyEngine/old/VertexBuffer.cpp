#include "stdafx.h"
#include "VertexBuffer.h"
#include "globals.h"

namespace sky
{
	VertexBuffer::VertexBuffer(): 
		m_APIVertexBuffer(nullptr)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_APIVertexBuffer)
		{
			delete m_APIVertexBuffer;
			m_APIVertexBuffer = nullptr;
		}
	}

	IAPIBuffer* VertexBuffer::getAPIVertexBuffer() const
	{
		return m_APIVertexBuffer;
	}

	StaticMeshVertexBuffer::StaticMeshVertexBuffer(): 
		rawData(nullptr),
		m_stride(0), 
		m_numVertices(0)
	{
	}

	StaticMeshVertexBuffer::~StaticMeshVertexBuffer()
	{
		if (rawData)
		{
			delete rawData;
			rawData = nullptr;
		}
	}

	void StaticMeshVertexBuffer::init(const vector<StaticMeshVertex>& vertices)
	{
		m_numVertices = vertices.size();

		m_stride = sizeof(StaticMeshVertex);

		rawData = new StaticMeshVertex[m_numVertices];

		const void* rawVertexData = vertices.data();

		memcpy_s(rawData, sizeof(StaticMeshVertex) * m_numVertices, rawVertexData, m_numVertices * sizeof(StaticMeshVertex));

		IDevice *device = sky::global::getDevice();

		assert(device != nullptr);

		m_APIVertexBuffer = device->createVertexBuffer(rawData, m_numVertices, m_stride);
	}

	unsigned int StaticMeshVertexBuffer::getStride() const
	{
		return m_stride;
	}

	size_t StaticMeshVertexBuffer::getVerticesCount() const
	{
		return m_numVertices;
	}

	const void* StaticMeshVertexBuffer::getRawData() const
	{
		return rawData;
	}

	ColorVertexBuffer::ColorVertexBuffer()
		:rawData(nullptr),
		m_stride(0),
		m_numVertices(0)
	{
	}

	ColorVertexBuffer::~ColorVertexBuffer()
	{
		if (rawData)
		{
			delete rawData;
			rawData = nullptr;
		}
	}

	void ColorVertexBuffer::init(const vector<ColorVertex>& vertices)
	{
		m_numVertices = vertices.size();

		m_stride = sizeof(ColorVertex);

		rawData = new ColorVertex[m_numVertices];

		const void* rawVertexData = vertices.data();

		memcpy_s(rawData, sizeof(ColorVertex) * m_numVertices, rawVertexData, m_numVertices * sizeof(ColorVertex));

		IDevice *device = sky::global::getDevice();

		assert(device != nullptr);

		m_APIVertexBuffer = device->createVertexBuffer(rawData, m_numVertices, m_stride);
	}

	unsigned int ColorVertexBuffer::getStride() const
	{
		return m_stride;
	}

	size_t ColorVertexBuffer::getVerticesCount() const
	{
		return m_numVertices;
	}

	const void* ColorVertexBuffer::getRawData() const
	{
		return rawData;
	}
}
