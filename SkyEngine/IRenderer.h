#pragma once
#include "RenderData.h"
#include "Device.h"

class IRenderer
{
public:
	virtual ~IRenderer(){}
	virtual void render(RenderData *data) = 0;
};

struct cbPerObject
{
	XMFLOAT4X4 WVP;
	XMFLOAT4X4 worldMatrix;
};

class Renderer: public IRenderer
{
public:
	virtual ~Renderer(){}
	Renderer(Device* pDevice)
		:_pDevice(pDevice)
	{

	}
	virtual void render(RenderData *data)
	{
		_pDevice->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::TRIANGLELIST);

		unsigned int renderPassCount = data->getRenderPassCount();

		for (size_t i = 0; i < renderPassCount; i++)
		{
			const RenderPass *pass = data->getRenderPass(i);

			static float t = 0.0f;
			t += 0.001;
			if (t > 6.28319)
			{
				t = 0;
			}

			data->setRotation(XMFLOAT3(0, t, 0));

			data->updateTransform();

			XMFLOAT4X4 WVP = data->getGlobalTransform();
			XMFLOAT4X4 world = data->getLocalTransform();

			cbPerObject cb;
			cb.WVP = WVP;
			cb.worldMatrix = world;

			InputLayout* il = data->getInputLayout();
			il->bind();

			VertexBuffer* vb = data->getVertexBuffer();
			vb->bind();

			IndexBuffer* ib = data->getIndexBuffer();
			ib->bind();

			ConstantBuffer* _cb = data->getConstantBuffer(0);
			_cb->bind(sky::EShaderType::STVertex);
			_cb->update(&cb);

			Shader *vShader = pass->getVertexShader();
			vShader->bind();

			Shader *pShader = pass->getPixelShader();
			pShader->bind();

			Texture *texture = data->getTexture(0);
			texture->bind(sky::EShaderType::STPixel);

			unsigned int indexCount = ib->getIndexCount();

			_pDevice->setDepthState(sky::EDepthState::DS_DEFAULT);

			_pDevice->setBackFaceCulling();

			_pDevice->setRepeatMaxAnisoSampler(sky::EShaderType::STPixel, 0);

			_pDevice->drawIndexed(indexCount, 0, 0);
		}
	};
protected:
	Device* _pDevice;
};
