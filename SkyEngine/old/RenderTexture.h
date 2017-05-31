#pragma once
#include "ITexture.h"
#include <d3d11.h>
#include "common.h"

namespace sky
{
	class RenderTexture :
		public ITexture
	{
	public:
		SKYENGINEDLL RenderTexture(int textureWidth, int textureHeight);
		SKYENGINEDLL ~RenderTexture();
		IRenderTargetResources *getRenderTargetResources() const;
		IShaderResource *getShaderResource() const override;
	private:
		IRenderTargetResources *m_renderTargetResources;
	};
}

