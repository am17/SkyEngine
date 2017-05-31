#pragma once
#include "IShader.h"
#include <d3d11.h>
#include <vector>

using namespace std;

namespace sky
{
	class PixelShader :
		public IPixelShader
	{
	public:
		PixelShader(const void* pByteCode, size_t byteCodeLength, size_t constantBufferCount, size_t shaderResourceCount);
		~PixelShader();
		void setResource(ID3D11PixelShader *data) override;
		ID3D11PixelShader *getResource() override;
		size_t getConstantBufferCount() const override;
		const void* getByteCode() const override;
		size_t getByteCodeLength() const override;
		size_t getShaderResourceCount() const override;
		void setConstantBufferSize(unsigned int bufferIndex, size_t bufferSize) override;
		size_t getConstantBufferSize(unsigned int bufferIndex) override;
	private:
		ID3D11PixelShader* m_pixelShader;
		size_t m_constantBufferCount;
		const void* pByteCode;
		size_t ByteCodeLength;
		size_t m_shaderResourceCount;
		vector<size_t> cbSizes;
	};
}

