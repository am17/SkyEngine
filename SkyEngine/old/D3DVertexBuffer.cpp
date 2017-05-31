#include "stdafx.h"
#include "D3DVertexBuffer.h"

namespace sky
{
	D3DVertexBuffer::D3DVertexBuffer()
		:m_vertexBuffer(nullptr),
		m_size(0),
		m_stride(0),
		m_elementsCount(0),
		m_pData(nullptr)
		
	{
	}


	D3DVertexBuffer::~D3DVertexBuffer()
	{
		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = nullptr;
		}
	}

	void D3DVertexBuffer::setResource(ID3D11Buffer *buffer)
	{
		m_vertexBuffer = buffer;
	}
	ID3D11Buffer * D3DVertexBuffer::getResource()
	{
		return m_vertexBuffer;
	}

	size_t D3DVertexBuffer::getSize() const
	{
		return m_size;
	}
	void D3DVertexBuffer::setSize(size_t size)
	{
		m_size = size;
	}

	unsigned int D3DVertexBuffer::getStride() const
	{
		return m_stride;
	}
	void D3DVertexBuffer::setStride(unsigned int stride)
	{
		m_stride = stride;
	}

	void D3DVertexBuffer::setElementsCount(unsigned int elementsCount)
	{
		m_elementsCount = elementsCount;
	}
	unsigned int D3DVertexBuffer::getElementsCount() const
	{
		return m_elementsCount;
	}

	void D3DVertexBuffer::setData(void* pData)
	{
		m_pData = pData;
	}
	void* D3DVertexBuffer::getData() const
	{
		return m_pData;
	}
}
