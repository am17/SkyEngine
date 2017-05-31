#pragma once
#include <d3d11.h>

namespace sky
{
	class IAPIBuffer
	{
	public:
		virtual ~IAPIBuffer() {}
		virtual void setResource(ID3D11Buffer *buffer) = 0;
		virtual ID3D11Buffer *getResource() = 0;
		virtual size_t getSize() const = 0;
		virtual void setSize(size_t size) = 0;
		virtual unsigned int getStride() const = 0;
		virtual void setStride(unsigned int stride) = 0;
		virtual void setElementsCount(unsigned int elementsCount) = 0;
		virtual unsigned int getElementsCount() const = 0;
		virtual void setData(void* pData) = 0;
		virtual void* getData() const = 0;
	};
}