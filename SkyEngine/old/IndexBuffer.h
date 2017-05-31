#pragma once
#include <vector>
#include "IAPIBuffer.h"

using namespace std;

namespace sky
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		virtual ~IndexBuffer();
		IAPIBuffer *getAPIIndexBuffer() const;
	protected:
		IAPIBuffer *m_APIIndexBuffer;
	};

	class StaticIndexBuffer : public IndexBuffer
	{
	public:
		StaticIndexBuffer();
		~StaticIndexBuffer();
		void init(const vector<unsigned int>& indices);
		unsigned int getNumIndeces() const;
		const void* getRawData() const;
		unsigned int getStride() const;
	private:
		void* rawData;
		unsigned int m_stride;
		size_t m_numIndices;
	};
}
