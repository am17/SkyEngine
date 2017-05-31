#pragma once
#include "IShader.h"
#include <d3d11.h>
#include <vector>

using namespace std;

namespace sky
{
	class VertexShader :
		public IVertexShader
	{
	public:
		VertexShader(const void* pByteCode, size_t byteCodeLength, size_t constantBufferCount, size_t vertexBufferCount);
		~VertexShader();
		void setResource(ID3D11VertexShader* data) override;
		ID3D11VertexShader *getResource() override;
		size_t getConstantBufferCount() const override;
		const void* getByteCode() const override;
		size_t getByteCodeLength() const override;
		VertexInputLayout *getInputSignature() const override;
		void setInputSignature(VertexInputLayout *pVertexDeclaration) override;
		size_t getVertexBufferCount() const override;
		void setConstantBufferSize(unsigned int bufferIndex, size_t bufferSize) override;
		size_t getConstantBufferSize(unsigned int bufferIndex) override;
	private:
		ID3D11VertexShader* m_vertexShader;
		const size_t m_constantBufferCount;
		const size_t m_vertexBufferCount;
		const void* pByteCode;
		size_t ByteCodeLength;
		VertexInputLayout *m_inputSignature;
		vector<size_t> cbSizes;
	};
}

