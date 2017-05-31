#include "stdafx.h"
#include "PixelShader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace sky
{
	PixelShader::PixelShader(const void* pByteCode, size_t byteCodeLength, size_t constantBufferCount, size_t shaderResourceCount)
		:m_pixelShader(nullptr),
		m_constantBufferCount(constantBufferCount),
		pByteCode(pByteCode),
		ByteCodeLength(byteCodeLength), 
		m_shaderResourceCount(shaderResourceCount)
	{
		cbSizes.reserve(constantBufferCount);
		cbSizes.resize(constantBufferCount);
	}


	PixelShader::~PixelShader()
	{
		if (m_pixelShader)
		{
			m_pixelShader->Release();
			m_pixelShader = nullptr;
		}
	}

	void PixelShader::setResource(ID3D11PixelShader *data)
	{
		m_pixelShader = data;
	}

	ID3D11PixelShader * PixelShader::getResource()
	{
		return m_pixelShader;
	}

	size_t  PixelShader::getConstantBufferCount() const
	{
		return m_constantBufferCount;
	}

	const void* PixelShader::getByteCode() const
	{
		return pByteCode;
	}

	size_t PixelShader::getByteCodeLength() const
	{
		return ByteCodeLength;
	}

	size_t PixelShader::getShaderResourceCount() const
	{
		return m_shaderResourceCount;
	}

	void PixelShader::setConstantBufferSize(unsigned bufferIndex, size_t bufferSize)
	{
		assert(bufferIndex < m_constantBufferCount);

		cbSizes[bufferIndex] = bufferSize;
	}

	size_t PixelShader::getConstantBufferSize(unsigned bufferIndex)
	{
		assert(bufferIndex < m_constantBufferCount);

		return cbSizes[bufferIndex];
	}
}
