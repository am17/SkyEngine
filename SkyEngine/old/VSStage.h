#pragma once
#include "IShader.h"
#include "ConstantBuffer.h"
#include <vector>

using namespace std;

namespace sky
{
	class VSStage
	{
	public:
		VSStage();
		~VSStage();
		void setVertexShader(IVertexShader* shader);
		IVertexShader* getVertexShader() const;
		ConstantBuffer *getConstantBuffer(unsigned int bufferIndex) const;
		void setConstantBuffer(IAPIBuffer *buffer, unsigned int bufferIndex);
	private:
		void deleteCurrentConstantBuffer();
		IVertexShader *vertexShader;
		vector<ConstantBuffer *> constantBuffers;
		vector<ID3D11Buffer *> constantBuffersResources;
		size_t m_constantBufferCount;
	};
}

