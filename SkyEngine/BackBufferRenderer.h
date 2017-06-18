#pragma once
#include "IRenderer.h"
#include "ShaderFactory.h"

class BackBufferRenderer : public Renderer
{
public:
	BackBufferRenderer(Device* pDevice)
		:Renderer(pDevice), _pass(nullptr)
	{
		_pShaderFactory = new ShaderFactory(_pDevice);

		_pass = new RenderPass();

		Shader *vertexShader = _pShaderFactory->getShader(L"mainVS.hlsl", sky::EShaderType::STVertex, "FullScreenQuadVS");
		Shader *pixelShader = _pShaderFactory->getShader(L"mainPS.hlsl", sky::EShaderType::STPixel, "MainToBackBufferPS");

		_pass->init(vertexShader, nullptr, nullptr, nullptr, pixelShader);
	}
	virtual ~BackBufferRenderer()
	{
		if (_pass)
		{
			delete _pass;
			_pass = nullptr;
		}

		if (_pShaderFactory)
		{
			delete _pShaderFactory;
			_pShaderFactory = nullptr;
		}
	}
	virtual void render(RenderData *data) override
	{
		_pDevice->setRenderTarget(ERenderTarget::RT_BACK_BUFFER);

		_pDevice->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::TRIANGLESTRIP);

		_pDevice->setBackFaceCulling(false);
		_pDevice->setDepthState(sky::EDepthState::DS_DEFAULT);

		_pass->apply();

		_pDevice->draw(4);
	}
protected:
	RenderPass *_pass;
	ShaderFactory *_pShaderFactory;
};

