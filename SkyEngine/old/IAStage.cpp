#include "stdafx.h"
#include "IAStage.h"
#include <cassert>

namespace sky
{
	IAStage::IAStage()
		:indexBuffer(nullptr),
		inputLayout(nullptr),
		m_PrimitiveTopology(PRIMITIVE_TOPOLOGY::UNKNOWN),
		m_vertexBuffersCount(0)
	{
		
	}


	IAStage::~IAStage()
	{
		vertexBuffers.clear();
	}

	IAPIBuffer* IAStage::getVertexBuffer(unsigned bufferIndex) const
	{
		assert(bufferIndex < m_vertexBuffersCount);

		return vertexBuffers[bufferIndex];
	}

	void IAStage::setVertexBuffer(IAPIBuffer* buffer, unsigned bufferIndex)
	{
		assert(bufferIndex < m_vertexBuffersCount);

		vertexBuffers[bufferIndex] = buffer;
	}

	void IAStage::setVertexBufferCount(size_t vertexBuffersCount)
	{
		vertexBuffers.reserve(vertexBuffersCount);
		vertexBuffers.resize(vertexBuffersCount);

		m_vertexBuffersCount = vertexBuffersCount;
	}

	void IAStage::setIndexBuffer(IAPIBuffer *buffer)
	{
		indexBuffer = buffer;
	}
	IAPIBuffer * IAStage::getIndexBuffer() const
	{
		return indexBuffer;
	}
	void IAStage::setInputLayout(VertexInputLayout *layout)
	{
		inputLayout = layout;
	}
	VertexInputLayout * IAStage::getInputLayout() const
	{
		return inputLayout;
	}
	void IAStage::setPrimitiveTopology(PRIMITIVE_TOPOLOGY topology)
	{
		m_PrimitiveTopology = topology;
	}
	PRIMITIVE_TOPOLOGY IAStage::getPrimitiveTopology() const
	{
		return m_PrimitiveTopology;
	}

	size_t IAStage::getVertexBuffersCount() const
	{
		return m_vertexBuffersCount;
	}
}
