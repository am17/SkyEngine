#pragma once
#include "IShader.h"
#include  "ConstantBuffer.h"
#include "ITexture.h"
#include <vector>

using namespace std;

namespace sky
{
	class PSStage
	{
	public:
		PSStage();
		~PSStage();
		void setPixelShader(IPixelShader* shader);
		IPixelShader* getPixelShader() const;
		ConstantBuffer *getConstantBuffer(unsigned int bufferIndex) const;
		void setConstantBuffer(IAPIBuffer *buffer, unsigned int bufferIndex);
		IShaderResource *getShaderResourcer(unsigned int resourceIndex) const;
		void setShaderResource(IShaderResource *buffer, unsigned int resourceIndex);
	private:
		void deleteCurrentConstantBuffer();
		IPixelShader *pixelShader;
		vector<ConstantBuffer *> constantBuffers;
		size_t m_constantBufferCount;
		vector<IShaderResource *> m_shaderResources;
		size_t m_shaderResourcesCount;
	};
}

