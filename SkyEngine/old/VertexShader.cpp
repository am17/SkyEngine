#include "stdafx.h"
#include "VertexShader.h"
#include <d3dcompiler.h>
#include "ConstantBuffer.h"

#pragma comment(lib, "D3DCompiler.lib")

namespace sky
{
	VertexShader::VertexShader(const void* pByteCode, size_t byteCodeLength, size_t constantBufferCount, size_t vertexBufferCount)
		:m_vertexShader(nullptr),
		m_constantBufferCount(constantBufferCount),
		m_vertexBufferCount(vertexBufferCount),
		pByteCode(pByteCode),
		ByteCodeLength(byteCodeLength), 
		m_inputSignature(nullptr)
	{
		cbSizes.reserve(constantBufferCount);
		cbSizes.resize(constantBufferCount);
	}


	VertexShader::~VertexShader()
	{
		if (m_vertexShader)
		{
			m_vertexShader->Release();
			m_vertexShader = nullptr;
		}
	}

	void VertexShader::setResource(ID3D11VertexShader* data)
	{
		m_vertexShader = data;
	}

	ID3D11VertexShader * VertexShader::getResource()
	{
		return m_vertexShader;
	}

	size_t  VertexShader::getConstantBufferCount() const
	{
		return m_constantBufferCount;
	}

	const void* VertexShader::getByteCode() const
	{
		return pByteCode;
	}

	size_t VertexShader::getByteCodeLength() const
	{
		return ByteCodeLength;
	}

	VertexInputLayout* VertexShader::getInputSignature() const
	{
		return m_inputSignature;
	}

	void VertexShader::setInputSignature(VertexInputLayout* pVertexDeclaration)
	{
		m_inputSignature = pVertexDeclaration;
	}

	size_t VertexShader::getVertexBufferCount() const
	{
		return m_vertexBufferCount;
	}

	void VertexShader::setConstantBufferSize(unsigned bufferIndex, size_t bufferSize)
	{
		assert(bufferIndex < m_constantBufferCount);

		cbSizes[bufferIndex] = bufferSize;
	}

	size_t VertexShader::getConstantBufferSize(unsigned bufferIndex)
	{
		assert(bufferIndex < m_constantBufferCount);

		return cbSizes[bufferIndex];
	}
}
