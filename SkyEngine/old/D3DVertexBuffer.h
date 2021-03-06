#pragma once
#include <d3d11.h>
#include "IAPIBuffer.h"

namespace sky
{
	class D3DVertexBuffer : public IAPIBuffer
	{
	public:
		D3DVertexBuffer();
		~D3DVertexBuffer();
		void setResource(ID3D11Buffer *buffer) override;
		ID3D11Buffer *getResource() override;
		size_t getSize() const override;
		void setSize(size_t size) override;
		unsigned int getStride() const override;
		void setStride(unsigned int stride) override;
		void setElementsCount(unsigned int elementsCount) override;
		unsigned int getElementsCount() const override;
		void setData(void* pData) override;
		void* getData() const override;
	private:
		ID3D11Buffer* m_vertexBuffer;
		size_t m_size;
		unsigned int m_stride;
		unsigned int m_elementsCount;
		void *m_pData;
	};
}

