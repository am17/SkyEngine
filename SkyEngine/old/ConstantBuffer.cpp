#include "stdafx.h"
#include "ConstantBuffer.h"
#include <cassert>

namespace sky
{
	ConstantBuffer::ConstantBuffer()
		:m_constantBuffer(nullptr),
		m_size(0),
		m_elementsCount(0),
		m_pData(nullptr)
	{
	}


	ConstantBuffer::~ConstantBuffer()
	{
		if (m_constantBuffer)
		{
			m_constantBuffer->Release();
			m_constantBuffer = nullptr;
		}
	}

	void ConstantBuffer::setResource(ID3D11Buffer *buffer)
	{
		m_constantBuffer = buffer;
	}

	ID3D11Buffer * ConstantBuffer::getResource()
	{
		return m_constantBuffer;
	}

	size_t ConstantBuffer::getSize() const
	{
		return m_size;
	}
	void ConstantBuffer::setSize(size_t size)
	{
		m_size = size;

		//m_pData = malloc(size);
	}

	unsigned int ConstantBuffer::getStride() const
	{
		return 0;
	}
	void ConstantBuffer::setStride(unsigned int stride)
	{

	}

	void ConstantBuffer::setElementsCount(unsigned int elementsCount)
	{
		m_elementsCount = elementsCount;
	}
	unsigned int ConstantBuffer::getElementsCount() const
	{
		return m_elementsCount;
	}

	void ConstantBuffer::setData(void* pData)
	{
		//memcpy_s(m_pData, m_size, pData, m_size);
		m_pData = pData;
	}
	void* ConstantBuffer::getData() const
	{
		//assert(m_pData != nullptr);

		return m_pData;
	}
}
