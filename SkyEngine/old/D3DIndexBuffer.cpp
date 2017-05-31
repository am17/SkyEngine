#include "stdafx.h"
#include "D3DIndexBuffer.h"

namespace sky
{
	D3DIndexBuffer::D3DIndexBuffer()
		:m_indexBuffer(nullptr),
		m_size(0), 
		m_elementsCount(0),
		m_pData(nullptr)
	{
	}


	D3DIndexBuffer::~D3DIndexBuffer()
	{
		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = nullptr;
		}
	}

	void D3DIndexBuffer::setResource(ID3D11Buffer *buffer)
	{
		m_indexBuffer = buffer;
	}
	ID3D11Buffer * D3DIndexBuffer::getResource()
	{
		return m_indexBuffer;
	}

	size_t D3DIndexBuffer::getSize() const
	{
		return m_size;
	}
	void D3DIndexBuffer::setSize(size_t size)
	{
		m_size = size;
	}

	unsigned int D3DIndexBuffer::getStride() const
	{
		return 0;
	}
	void D3DIndexBuffer::setStride(unsigned int stride)
	{

	}

	void D3DIndexBuffer::setElementsCount(unsigned int elementsCount)
	{
		m_elementsCount = elementsCount;
	}
	unsigned int D3DIndexBuffer::getElementsCount() const
	{
		return m_elementsCount;
	}
	void D3DIndexBuffer::setData(void* pData)
	{
		m_pData = pData;
	}
	void* D3DIndexBuffer::getData() const
	{
		return m_pData;
	}
}
