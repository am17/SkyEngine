#include "stdafx.h"
#include "RenderTexture.h"
#include "globals.h"

namespace sky
{
	RenderTexture::RenderTexture(int textureWidth, int textureHeight)
	{
		IDevice *device = global::getDevice();

		assert(device != nullptr);

		m_renderTargetResources = device->createRenderTargetResources(textureWidth, textureHeight);
	}

	RenderTexture::~RenderTexture()
	{
		if (m_renderTargetResources)
		{
			delete m_renderTargetResources;
			m_renderTargetResources = nullptr;
		}
	}

	IRenderTargetResources *RenderTexture::getRenderTargetResources() const
	{
		return m_renderTargetResources;
	}

	IShaderResource* RenderTexture::getShaderResource() const
	{
		assert(m_renderTargetResources != nullptr);

		return m_renderTargetResources->getShaderResource();
	}
}
