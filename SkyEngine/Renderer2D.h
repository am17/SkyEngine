#pragma once
#include "IRenderer.h"

class Renderer2D : public IRenderer
{
public:
	virtual ~Renderer2D(){}
	Renderer2D(Device* pDevice)
		:_pDevice(pDevice)
	{

	}
	virtual void render(RenderData *data)
	{
		unsigned int renderPassCount = data->getRenderPassCount();

		for (size_t i = 0; i < renderPassCount; i++)
		{
			const RenderPass *pass = data->getRenderPass(i);

			ERenderTarget renderTarget = pass->getRenderTarget();
			_pDevice->setRenderTarget(renderTarget);

			_pDevice->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::TRIANGLELIST);

			InputLayout* il = data->getInputLayout();
			il->bind();

			VertexBuffer* vb = data->getVertexBuffer();
			vb->bind();

			IndexBuffer* ib = data->getIndexBuffer();
			ib->bind();

			Shader *vShader = pass->getVertexShader();
			vShader->bind();

			Shader *pShader = pass->getPixelShader();
			pShader->bind();

			Texture *texture = _pDevice->getDepthMap();// data->getTexture(0);

			//texture->saveToFile(L"_DepthMap.dds");

			texture->bind(sky::EShaderType::STPixel, 0);

			unsigned int indexCount = ib->getIndexCount();

			_pDevice->setDepthState(sky::EDepthState::DS_NONE);
			_pDevice->setBlendState(sky::EBlendState::BS_OPAQUE);

			_pDevice->setBackFaceCulling(false);

			_pDevice->setRepeatMaxAnisoSampler(sky::EShaderType::STPixel, 0);

			_pDevice->drawIndexed(indexCount, 0, 0);
		}
	};
protected:
	Device* _pDevice;
};