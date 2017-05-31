#pragma once
#include <d3d11.h>
#include "VertexInputLayout.h"

namespace sky
{
	class IVertexShader
	{
	public:
		virtual ~IVertexShader() {}
		virtual void setResource(ID3D11VertexShader *data) = 0;
		virtual ID3D11VertexShader *getResource() = 0;
		virtual size_t getConstantBufferCount() const = 0;
		virtual const void* getByteCode() const = 0;
		virtual size_t getByteCodeLength() const = 0;
		virtual VertexInputLayout *getInputSignature() const = 0;
		virtual void setInputSignature(VertexInputLayout *pVertexDeclaration) = 0;
		virtual size_t getVertexBufferCount() const = 0;
		virtual void setConstantBufferSize(unsigned int bufferIndex, size_t bufferSize) = 0;
		virtual size_t getConstantBufferSize(unsigned int bufferIndex) = 0;
	};

	class IPixelShader
	{
	public:
		virtual ~IPixelShader() {}
		virtual void setResource(ID3D11PixelShader *data) = 0;
		virtual ID3D11PixelShader *getResource() = 0;
		virtual size_t getConstantBufferCount() const = 0;
		virtual const void* getByteCode() const = 0;
		virtual size_t getByteCodeLength() const = 0;
		virtual size_t getShaderResourceCount() const = 0;
		virtual void setConstantBufferSize(unsigned int bufferIndex, size_t bufferSize) = 0;
		virtual size_t getConstantBufferSize(unsigned int bufferIndex) = 0;
	};
}