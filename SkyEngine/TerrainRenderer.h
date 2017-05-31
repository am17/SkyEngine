#pragma once
#include "IRenderer.h"
#include "TileRing.h"

struct cbPerFrame
{
	XMFLOAT4X4 g_WorldViewProj;
	XMFLOAT3 g_TextureWorldOffset;
	float g_tileSize;
};
struct cbPerFrame4
{
	XMFLOAT3 g_TextureWorldOffset;
	float g_CoarseSampleSpacing;
};

class TerrainRenderer : public Renderer
{
public:
	TerrainRenderer(Device* pDevice, int nRings, TileRing **aTileRings, cbPerFrame *aPerFrameBuffer, cbPerFrame4 *aPerFrameBuffer4);
	~TerrainRenderer();
	virtual void render(RenderData *data) override;
private:
	int _nRings;
	TileRing **_pTileRings;
	cbPerFrame *_perFrameBuffer;
	cbPerFrame4 *_perFrameBuffer4;
};

TerrainRenderer::TerrainRenderer(Device* pDevice, int nRings, TileRing **aTileRings, cbPerFrame *aPerFrameBuffer, cbPerFrame4 *aPerFrameBuffer4)
	:Renderer(pDevice),
	_nRings(nRings),
	_pTileRings(aTileRings),
	_perFrameBuffer(aPerFrameBuffer),
	_perFrameBuffer4(aPerFrameBuffer4)
{
}

TerrainRenderer::~TerrainRenderer()
{
}

void TerrainRenderer::render(RenderData *data)
{
	unsigned int renderPassCount = data->getRenderPassCount();

	for (size_t i = 0; i < renderPassCount; i++)
	{
		const RenderPass *pass = data->getRenderPass(i);

		ERenderTarget renderTarget = pass->getRenderTarget();
		_pDevice->setRenderTarget(renderTarget);

		_pDevice->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::TRIANGLESTRIP);

		data->getInputLayout()->bind();

		data->getIndexBuffer()->bind();

		data->getConstantBuffer(0)->bind(sky::EShaderType::STVertex, 0);

		Shader* vertexShader = pass->getVertexShader();
		Shader* pixelShader = pass->getPixelShader();

		vertexShader->bind();
		pixelShader->bind();

		data->getTexture(0)->bind(sky::EShaderType::STVertex, 0);


		data->getConstantBuffer(1)->bind(sky::EShaderType::STPixel, 0);

		data->getTexture(1)->bind(sky::EShaderType::STPixel, 0);
		data->getTexture(2)->bind(sky::EShaderType::STPixel, 1);
		data->getTexture(3)->bind(sky::EShaderType::STPixel, 2);
		data->getTexture(4)->bind(sky::EShaderType::STPixel, 3);

		_pDevice->setRepeatMaxAnisoSampler(sky::EShaderType::STPixel, 0);
		_pDevice->setClampLinearSampler(sky::EShaderType::STPixel, 1);

		_pDevice->setClampLinearSampler(sky::EShaderType::STVertex, 0);

		data->getConstantBuffer(1)->update(_perFrameBuffer4);

		_pDevice->setBackFaceCulling();
		_pDevice->setDepthState(sky::EDepthState::DS_DEFAULT);
		_pDevice->setBlendState(sky::EBlendState::BS_OPAQUE);

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