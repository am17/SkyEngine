#pragma once
#include "IRenderer.h"
#include "TileRing.h"

struct Const_Per_Call
{
	XMFLOAT4X4 g_matLocal;
	XMFLOAT4X4 g_matWorldViewProj;
	XMFLOAT2 g_PerlinMovement;
	float g_tileSize;
	float unuse1;
};

struct cbPerFrame41
{
	XMFLOAT3 g_TextureWorldOffset;
	float unuse1;
	XMFLOAT3 cameraPosition;
	float unuse2;
};

class OceanRenderer : public Renderer
{
public:
	OceanRenderer(Device* pDevice, int nRings, TileRing **aTileRings, Const_Per_Call *aPerFrameBuffer, cbPerFrame41 *aPerFrameBuffer4);
	~OceanRenderer();
	virtual void render(RenderData *data) override;
private:
	int _nRings;
	TileRing **_pTileRings;
	Const_Per_Call *_perFrameBuffer;
	cbPerFrame41 *_perFrameBuffer4;
};

OceanRenderer::OceanRenderer(Device* pDevice, int nRings, TileRing **aTileRings, Const_Per_Call *aPerFrameBuffer, cbPerFrame41 *aPerFrameBuffer4)
	:Renderer(pDevice),
	_nRings(nRings),
	_pTileRings(aTileRings),
	_perFrameBuffer(aPerFrameBuffer),
	_perFrameBuffer4(aPerFrameBuffer4)
{
}

OceanRenderer::~OceanRenderer()
{
}

void OceanRenderer::render(RenderData *data)
{
	_pDevice->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::TRIANGLESTRIP);


	unsigned int renderPassCount = data->getRenderPassCount();

	for (size_t i = 0; i < renderPassCount; i++)
	{
		const RenderPass *pass = data->getRenderPass(i);

		ERenderTarget renderTarget = pass->getRenderTarget();
		_pDevice->setRenderTarget(renderTarget);

		data->getInputLayout()->bind();

		data->getIndexBuffer()->bind();

		data->getConstantBuffer(0)->bind(sky::EShaderType::STVertex, 0);
		data->getConstantBuffer(1)->bind(sky::EShaderType::STVertex, 1);
		pass->getVertexShader()->bind();
		pass->getPixelShader()->bind();

		data->getConstantBuffer(0)->bind(sky::EShaderType::STPixel, 0);
		data->getConstantBuffer(1)->bind(sky::EShaderType::STPixel, 1);

		data->getTexture(0)->bind(sky::EShaderType::STPixel, 0);
		data->getTexture(1)->bind(sky::EShaderType::STPixel, 1);
		data->getTexture(2)->bind(sky::EShaderType::STPixel, 2);
		data->getTexture(3)->bind(sky::EShaderType::STPixel, 3);
		_pDevice->getDepthMap()->bind(sky::EShaderType::STPixel, 4);

		//Texture *depthMap = _pDevice->getDepthMap();

		_pDevice->setRepeatMaxAnisoSampler(sky::EShaderType::STPixel, 0);
		_pDevice->setClampLinearSampler(sky::EShaderType::STPixel, 1);
		_pDevice->setRepeatMaxAnisoSampler(sky::EShaderType::STPixel, 2);
		_pDevice->setRepeatMaxAnisoSampler(sky::EShaderType::STPixel, 3);

		data->getConstantBuffer(1)->update(_perFrameBuffer4);

		_pDevice->setBackFaceCulling();
		_pDevice->setDepthState(sky::EDepthState::DS_DEFAULT);
		//_pDevice->setBlendState(sky::EBlendState::BS_ALPHA_BLENDING);

		for (size_t i = 0; i != _nRings; ++i)
		{
			const TileRing* pRing = _pTileRings[i];
			pRing->setRenderingState();

			_perFrameBuffer->g_tileSize = pRing->tileSize();

			const int nIndices = data->getIndexBuffer()->getIndexCount();
			const int InstanceCount = pRing->nTiles();

			data->getConstantBuffer(0)->update(_perFrameBuffer);

			_pDevice->drawIndexedInstanced(nIndices, InstanceCount, 0, 0, 0);
		}
	}
}