#include "stdafx.h"
#include "PSStage.h"
#include "globals.h"

namespace sky
{
	PSStage::PSStage()
		:pixelShader(nullptr), 
		m_constantBufferCount(0), 
		m_shaderResourcesCount(0)
	{
	}


	PSStage::~PSStage()
	{
		deleteCurrentConstantBuffer();
	}

	void PSStage::setPixelShader(IPixelShader* shader)
	{
		pixelShader = shader;

		m_constantBufferCount = shader->getConstantBufferCount();

		deleteCurrentConstantBuffer();

		constantBuffers.reserve(m_constantBufferCount);
		constantBuffers.resize(m_constantBufferCount);

		for (size_t i = 0; i < m_constantBufferCount; i++)
		{
			size_t bufferSize = pixelShader->getConstantBufferSize(i);

			IAPIBuffer *cbbuffer = global::getDevice()->createConstantBuffer(bufferSize);

			setConstantBuffer(cbbuffer, i);
		}

		m_shaderResourcesCount = shader->getShaderResourceCount();

		m_shaderResources.reserve(m_shaderResourcesCount);
		m_shaderResources.resize(m_shaderResourcesCount);
	}
	IPixelShader* PSStage::getPixelShader() const
	{
		return pixelShader;
	}

	ConstantBuffer* PSStage::getConstantBuffer(unsigned bufferIndex) const
	{
		assert(bufferIndex < m_constantBufferCount);

		return constantBuffers[bufferIndex];
	}

	void PSStage::setConstantBuffer(IAPIBuffer* buffer, unsigned bufferIndex)
	{
		assert(bufferIndex < m_constantBufferCount);

		constantBuffers[bufferIndex] = static_cast<ConstantBuffer *>(buffer);
	}

	IShaderResource* PSStage::getShaderResourcer(unsigned resourceIndex) const
	{
		assert(resourceIndex < m_shaderResourcesCount);

		IShaderResource* rec = m_shaderResources[resourceIndex];

		return rec;
	}

	void PSStage::setShaderResource(IShaderResource* shaderResources, unsigned resourceIndex)
	{
		assert(resourceIndex < m_shaderResourcesCount);

		m_shaderResources[resourceIndex] = shaderResources;
	}

	void PSStage::deleteCurrentConstantBuffer()
	{
		if (constantBuffers.size() > 0)
		{
			for (vector<ConstantBuffer *>::const_iterator it = constantBuffers.begin(); it != constantBuffers.end(); ++it)
			{
				ConstantBuffer *cb = (*it);
				if (cb)
				{
					delete cb;
					cb = nullptr;
				}
			}

			constantBuffers.clear();
		}
	}
}
