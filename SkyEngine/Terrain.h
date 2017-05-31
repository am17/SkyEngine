#pragma once
#include "Device.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShaderFactory.h"
#include "InputLayout.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Camera.h"
#include "EntityDecorator.h"
#include "TerrainRenderer.h"
#include <DirectXMath.h>

using namespace DirectX;

class Terrain : public EntityDecorator
{
public:
	SKYENGINEDLL Terrain(Device *aDevice, Entity *entity, Camera *aCamera);
	~Terrain();
	SKYENGINEDLL void init();
	void update() override;
private:
	void createIB();
	void setUVOffset(XMFLOAT3 &pVar);
	void setMatrices();
	void initPass1();

	int TRI_STRIP_INDEX_COUNT;
	const int VTX_PER_TILE_EDGE;
	Device *_pDevice;
	ShaderFactory *_pShaderFactory;
	Camera *_pCamera;
	RenderData *_renderData;
	TileRing *pTileRings[10];
	cbPerFrame perFrameBuffer;
	cbPerFrame4 perFrameBuffer4;
	//XMFLOAT4X4 world;
	int nRings;
	float WORLD_SCALE;
	int COARSE_HEIGHT_MAP_SIZE;
	float SNAP_GRID_SIZE;
};

