#include "stdafx.h"
#include "Ocean.h"


Ocean::Ocean(Device *aDevice, Entity *entity, Camera *aCamera)
	:EntityDecorator(entity),
	VTX_PER_TILE_EDGE(9),
	_pDevice(aDevice),
	_pCamera(aCamera)
{
	_pShaderFactory = new ShaderFactory(_pDevice);
}


Ocean::~Ocean()
{
	if (_renderData)
	{
		delete _renderData;
		_renderData = nullptr;
	}
}

void Ocean::init()
{
	WORLD_SCALE = 4000;

	int widths[] = { 0, 16, 16, 16, 16 };
	nRings = sizeof(widths) / sizeof(widths[0]) - 1;

	assert(nRings <= 10);

	float tileWidth = 0.125f;
	for (int i = 0; i != nRings && i != 10; ++i)
	{
		pTileRings[i] = new TileRing(widths[i] / 2, widths[i + 1], tileWidth, _pDevice);
		tileWidth *= 2.0f;
	}

	const int PATCHES_PER_TILE_EDGE = VTX_PER_TILE_EDGE - 1;
	SNAP_GRID_SIZE = WORLD_SCALE * pTileRings[nRings - 1]->tileSize() / PATCHES_PER_TILE_EDGE;

	_renderData = new RenderData();

	createIB();

	Shader *vertexShader = _pShaderFactory->getShader(L"oceanVS.hlsl", sky::EShaderType::STVertex);
	Shader *pixelShader = _pShaderFactory->getShader(L"oceanPS.hlsl", sky::EShaderType::STPixel);

	RenderPass *renderPass1 = new RenderPass();
	renderPass1->init(vertexShader, nullptr, nullptr, nullptr, pixelShader);

	_renderData->addRenderPass(renderPass1);

	VertexLayoutDesc* layoutDesc = new VertexLayoutDesc[1];
	layoutDesc[0].SemanticName = "POSITION_2D";
	layoutDesc[0].SemanticIndex = 0;
	layoutDesc[0].Format = ELEMENT_FORMAT::FLOAT2;
	layoutDesc[0].InputSlot = 0;
	layoutDesc[0].ForInstance = true;

	InputLayout *inputLayout = _pDevice->createInputLayout(layoutDesc, 1, vertexShader->getByteCode(), vertexShader->getCodeLength());
	_renderData->setInputLayout(inputLayout);

	ConstantBuffer *constantBuffer = _pDevice->createConstantBuffer(sizeof(Const_Per_Call), EConstantBufferType::CBT_LongLived);
	ConstantBuffer *constantBuffer4 = _pDevice->createConstantBuffer(sizeof(cbPerFrame41), EConstantBufferType::CBT_LongLived);

	_renderData->addConstantBuffer(constantBuffer);
	_renderData->addConstantBuffer(constantBuffer4);

	Texture *perlin_noise = _pDevice->createTextureFromFile(L"Textures/perlin_noise.dds");
	Texture *fresnelMap = _pDevice->createFresnelMap();
	Texture *reflect_cube = _pDevice->createTextureFromFile(L"Textures/skymap_reflect.dds");
	Texture *gradient_map = _pDevice->createTextureFromFile(L"Textures/gradient_map.dds");

	_renderData->addTexture(perlin_noise);
	_renderData->addTexture(fresnelMap);
	_renderData->addTexture(reflect_cube);
	_renderData->addTexture(gradient_map);

	IRenderer* render = new OceanRenderer(_pDevice, nRings, pTileRings, &perFrameBuffer, &perFrameBuffer4);

	MeshComponent* grComponent = new MeshComponent(_renderData, render);

	_entity->add(grComponent);
}

void Ocean::createIB()
{
	TRI_STRIP_INDEX_COUNT = (VTX_PER_TILE_EDGE - 1) * (2 * VTX_PER_TILE_EDGE + 2);

	int index = 0;
	unsigned long* pIndices = new unsigned long[TRI_STRIP_INDEX_COUNT];

	for (int y = 0; y < VTX_PER_TILE_EDGE - 1; ++y)
	{
		const int rowStart = y * VTX_PER_TILE_EDGE;

		for (int x = 0; x < VTX_PER_TILE_EDGE; ++x)
		{
			pIndices[index++] = rowStart + x;
			pIndices[index++] = rowStart + x + VTX_PER_TILE_EDGE;
		}

		pIndices[index] = pIndices[index - 1];
		++index;
		pIndices[index++] = rowStart + VTX_PER_TILE_EDGE;
	}
	assert(index == TRI_STRIP_INDEX_COUNT);

	IndexBuffer *IndexBuffer = _pDevice->createIndexBuffer(pIndices, TRI_STRIP_INDEX_COUNT, sizeof(unsigned long), INDEX_BUFFER_FORMAT::IB_FORMAT_32);
	_renderData->setIndexBuffer(IndexBuffer);

	if (pIndices)
	{
		delete[] pIndices;
		pIndices = nullptr;
	}
}

void Ocean::setUVOffset(XMFLOAT3 &pVar)
{
	XMFLOAT3 eye = _pCamera->getPosition();

	eye.y = 0;

	if (SNAP_GRID_SIZE > 0)
	{
		eye.x = floorf(eye.x / SNAP_GRID_SIZE) * SNAP_GRID_SIZE;
		eye.z = floorf(eye.z / SNAP_GRID_SIZE) * SNAP_GRID_SIZE;
	}

	XMVECTOR vEye = XMLoadFloat3(&eye);
	vEye /= WORLD_SCALE;

	XMStoreFloat3(&eye, vEye);

	eye.z *= -1;

	pVar = eye;
}
void Ocean::setMatrices()
{
	_renderData->setScale(XMFLOAT3(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE));

	XMFLOAT3 eye = _pCamera->getPosition();

	float snappedX = eye.x, snappedZ = eye.z;
	if (SNAP_GRID_SIZE > 0)
	{
		snappedX = floorf(snappedX / SNAP_GRID_SIZE) * SNAP_GRID_SIZE;
		snappedZ = floorf(snappedZ / SNAP_GRID_SIZE) * SNAP_GRID_SIZE;
	}
	const float dx = eye.x - snappedX;
	const float dz = eye.z - snappedZ;

	snappedX = eye.x - 2 * dx;
	snappedZ = eye.z - 2 * dz;

	_renderData->setTranslate(XMFLOAT3(snappedX, 0, snappedZ));

	_renderData->updateTransform();

	perFrameBuffer.g_matLocal = _renderData->getLocalTransform();
	perFrameBuffer.g_matWorldViewProj = _renderData->getGlobalTransform();
}

void Ocean::update()
{
	setUVOffset(perFrameBuffer4.g_TextureWorldOffset);
	setMatrices();

	float g_PerlinSpeed = 0.06f;
	XMFLOAT2 g_WindDir = XMFLOAT2(0.8f, 0.6f);
	XMVECTOR WindDir = XMLoadFloat2(&g_WindDir);
	XMVECTOR perlin_move = -WindDir * 1.41640496 * g_PerlinSpeed;
	XMStoreFloat2(&perFrameBuffer.g_PerlinMovement, perlin_move);

	perFrameBuffer4.cameraPosition = _pCamera->getPosition();

	EntityDecorator::update();
}