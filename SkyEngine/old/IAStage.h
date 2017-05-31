#pragma once
#include "VertexInputLayout.h"
#include "IAPIBuffer.h"
#include <vector>

using namespace std;

namespace sky
{
	enum class PRIMITIVE_TOPOLOGY
	{
		TRIANGLELIST = 0,
		LINELIST = 1,
		UNKNOWN = -1
	};

	class IAStage
	{
	public:
		IAStage();
		~IAStage();
		IAPIBuffer* getVertexBuffer(unsigned int bufferIndex) const;
		void setVertexBuffer(IAPIBuffer* buffer, unsigned bufferIndex);
		void setVertexBufferCount(size_t vertexBuffersCount);
		void setIndexBuffer(IAPIBuffer *buffer);
		IAPIBuffer * getIndexBuffer() const;
		void setInputLayout(VertexInputLayout *layout);
		VertexInputLayout * getInputLayout() const;
		void setPrimitiveTopology(PRIMITIVE_TOPOLOGY topology);
		PRIMITIVE_TOPOLOGY getPrimitiveTopology() const;
		size_t getVertexBuffersCount() const;
	private:
		vector<IAPIBuffer*> vertexBuffers;
		IAPIBuffer *indexBuffer;
		VertexInputLayout *inputLayout;
		PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
		size_t m_vertexBuffersCount;
	};
}

