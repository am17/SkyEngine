#include "stdafx.h"
#include "VSStage.h"
#include <cassert>
#include "globals.h"

namespace sky
{
	VSStage::VSStage()
		:vertexShader(nullptr), 
		m_constantBufferCount(0)
	{
	}


	VSStage::~VSStage()
	{
		deleteCurrentConstantBuffer();
	}

	void VSStage::setVertexShader(IVertexShader* shader)
	{
		vertexShader = shader;

		m_constantBufferCount = vertexShader->getConstantBufferCount();

		size_t cbCount = vertexShader->getConstantBufferCount();

		deleteCurrentConstantBuffer();

		constantBuffers.reserve(cbCount);
		constantBuffers.resize(cbCount);

		for (size_t i = 0; i < cbCount; i++)
		{
			size_t bufferSize = vertexShader->getConstantBufferSize(i);

			IAPIBuffer *cbbuffer = global::getDevice()->createConstantBuffer(bufferSize);

			assert(cbbuffer != nullptr);

			setConstantBuffer(cbbuffer, i);
		}
	}
	IVertexShader* VSStage::getVertexShader() const
	{
		return vertexShader;
	}

	ConstantBuffer* VSStage::getConstantBuffer(unsigned bufferIndex) const
	{
		assert(bufferIndex < m_constantBufferCount);

		return constantBuffers[bufferIndex];
	}

	void VSStage::setConstantBuffer(IAPIBuffer* buffer, unsigned bufferIndex)
	{
		assert(bufferIndex < m_constantBufferCount);

		constantBuffers[bufferIndex] = static_cast<ConstantBuffer *>(buffer);
	}

	void VSStage::deleteCurrentConstantBuffer()
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
