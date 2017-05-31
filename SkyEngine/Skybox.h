#pragma once
#include "common.h"
#include "EntityDecorator.h"
#include "Camera.h"
#include "ShaderFactory.h"


struct Vertex    //Overloaded Vertex Structure
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz){}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};

class Skybox : public EntityDecorator
{
public:
	SKYENGINEDLL Skybox(Device *aDevice, Entity *entity, Camera *aCamera);
	~Skybox();
	void update() override;
	void createSphere(int LatLines, int LongLines);
private:
	Device *_pDevice;
	Camera *_pCamera;
	RenderData *_renderData;
	ShaderFactory *_pShaderFactory;
};

class SkyboxRenderer : public Renderer
{
public:
	SkyboxRenderer(Device* pDevice)
		:Renderer(pDevice)
	{

	}

	void render(RenderData *data)
	{
		unsigned int renderPassCount = data->getRenderPassCount();

		for (size_t i = 0; i < renderPassCount; i++)
		{
			const RenderPass *pass = data->getRenderPass(i);

			data->updateTransform();

			XMFLOAT4X4 WVP = data->getGlobalTransform();
			XMFLOAT4X4 world = data->getLocalTransform();

			cbPerObject cb;
			cb.WVP = WVP;
			cb.worldMatrix = world;

			InputLayout* il = data->getInputLayout();
			il->bind();

			_pDevice->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::TRIANGLELIST);

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

			_pDevice->setDepthState(sky::EDepthState::DS_LESS_EQUAL);
			_pDevice->setBlendState(sky::EBlendState::BS_OPAQUE);

			_pDevice->setBackFaceCulling(false);

			_pDevice->setRepeatMaxAnisoSampler(sky::EShaderType::STPixel, 0);

			ERenderTarget renderTarget = pass->getRenderTarget();
			_pDevice->setRenderTarget(renderTarget);

			unsigned int indexCount = ib->getIndexCount();

			_pDevice->drawIndexed(indexCount, 0, 0);
		}
	}
private:

};

