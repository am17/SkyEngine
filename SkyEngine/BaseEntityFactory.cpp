#include "stdafx.h"
#include "BaseEntityFactory.h"
#include "RenderData.h"
#include"IRenderer.h"
#include "Renderer2D.h"
#include "MeshComponent.h"
#include "GeometricPrimitive.h"

#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

using namespace DirectX;

BaseEntityFactory::BaseEntityFactory(Device* device)
	:_pDevice(device)
{
	_pShaderFactory = new ShaderFactory(device);
}


BaseEntityFactory::~BaseEntityFactory()
{
	if (_pShaderFactory)
	{
		delete _pShaderFactory;
		_pShaderFactory = nullptr;
	}
}

Entity* BaseEntityFactory::createEntity()
{
	return new Entity();
}

Entity* BaseEntityFactory::createCubeEntity()
{
	vector<VertexPositionNormalTexture> vertices;
	vector<uint16_t> indices;

	GeometricPrimitive::CreateCube(vertices, indices, 1.0f, false);

	return initBaseEntity(vertices.data(), vertices.size(), indices.data(), indices.size());
}
Entity* BaseEntityFactory::createTeapotEntity()
{
	vector<VertexPositionNormalTexture> vertices;
	vector<uint16_t> indices;

	GeometricPrimitive::CreateTeapot(vertices, indices, 1.0f, 8, false);

	return initBaseEntity(vertices.data(), vertices.size(), indices.data(), indices.size());
}
Entity* BaseEntityFactory::createSphereEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createBoxEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createGeoSphereEntity()
{
	vector<VertexPositionNormalTexture> vertices;
	vector<uint16_t> indices;

	GeometricPrimitive::CreateGeoSphere(vertices, indices, 1.0f, 2, false);

	return initBaseEntity(vertices.data(), vertices.size(), indices.data(), indices.size());
}
Entity* BaseEntityFactory::createCylinderEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createConeEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createTorusEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createTetrahedronEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createOctahedronEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createDodecahedronEntity()
{
	return nullptr;
}
Entity* BaseEntityFactory::createIcosahedronEntity()
{
	return nullptr;
}

Entity*  BaseEntityFactory::createQuadEntity()
{
	struct Vertex2D    //Overloaded Vertex Structure
	{
		Vertex2D(){}
		Vertex2D(float x, float y, float z)
			: pos(x, y, z){}

		XMFLOAT3 pos;
	};

	Vertex2D v[] =
	{
		Vertex2D(-1.0f, 0.5f, 0),
		Vertex2D(-1.0f, 1.0f, 0),
		Vertex2D(-0.5f, 1.0f, 0),
		Vertex2D(-0.5f, 0.5f, 0)
	};

	uint16_t indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	VertexBuffer* vb = _pDevice->createVertexBuffer(v, 4, sizeof(Vertex2D));
	IndexBuffer* ib = _pDevice->createIndexBuffer(indices, 6, sizeof(uint16_t), INDEX_BUFFER_FORMAT::IB_FORMAT_16);

	Shader *vertexShader = _pShaderFactory->getShader(L"2dVS.hlsl", sky::EShaderType::STVertex);
	Shader *pixelShader = _pShaderFactory->getShader(L"2dPS.hlsl", sky::EShaderType::STPixel);

	VertexLayoutDesc* layoutDesc = new VertexLayoutDesc[1];
	layoutDesc[0].SemanticName = "POSITION";
	layoutDesc[0].InputSlot = 0;
	layoutDesc[0].Format = ELEMENT_FORMAT::FLOAT3;
	layoutDesc[0].SemanticIndex = 0;
	layoutDesc[0].ForInstance = false;

	InputLayout *inputLayout = _pDevice->createInputLayout(layoutDesc, 1, vertexShader->getByteCode(), vertexShader->getCodeLength());

	Texture *texture = _pDevice->createTextureFromFile(L"Textures/stone01.dds");

	Entity* entity = createEntity();

	RenderData* data = new RenderData();
	data->setVertexBuffer(vb);
	data->setIndexBuffer(ib);

	RenderPass *renderPass1 = new RenderPass();
	renderPass1->init(vertexShader, nullptr, nullptr, nullptr, pixelShader);

	data->addRenderPass(renderPass1);

	data->setInputLayout(inputLayout);
	data->addTexture(texture);
	IRenderer* render = new Renderer2D(_pDevice);

	MeshComponent* grComponent = new MeshComponent(data, render);

	entity->add(grComponent);

	return entity;
}

Entity* BaseEntityFactory::initBaseEntity(const void *pVBData, unsigned int vbElementsCount, const void *pIBData, unsigned int ibElementsCount)
{
	VertexBuffer* vb = _pDevice->createVertexBuffer(pVBData, vbElementsCount, sizeof(VertexPositionNormalTexture));
	IndexBuffer* ib = _pDevice->createIndexBuffer(pIBData, ibElementsCount, sizeof(uint16_t), INDEX_BUFFER_FORMAT::IB_FORMAT_16);

	Shader *vertexShader = _pShaderFactory->getShader(L"base2VS.hlsl", sky::EShaderType::STVertex);
	Shader *pixelShader = _pShaderFactory->getShader(L"base2PS.hlsl", sky::EShaderType::STPixel);

	VertexLayoutDesc* layoutDesc = new VertexLayoutDesc[3];
	layoutDesc[0].SemanticName = "POSITION";
	layoutDesc[0].InputSlot = 0;
	layoutDesc[0].Format = ELEMENT_FORMAT::FLOAT3;
	layoutDesc[0].SemanticIndex = 0;
	layoutDesc[0].ForInstance = false;
	layoutDesc[1].SemanticName = "NORMAL";
	layoutDesc[1].InputSlot = 0;
	layoutDesc[1].Format = ELEMENT_FORMAT::FLOAT3;
	layoutDesc[1].SemanticIndex = 0;
	layoutDesc[1].ForInstance = false;
	layoutDesc[2].SemanticName = "TEXCOORD";
	layoutDesc[2].InputSlot = 0;
	layoutDesc[2].Format = ELEMENT_FORMAT::FLOAT2;
	layoutDesc[2].SemanticIndex = 0;
	layoutDesc[2].ForInstance = false;

	InputLayout *inputLayout = _pDevice->createInputLayout(layoutDesc, 3, vertexShader->getByteCode(), vertexShader->getCodeLength());

	ConstantBuffer *constantBuffer = _pDevice->createConstantBuffer(sizeof(cbPerObject), EConstantBufferType::CBT_LongLived);

	Texture *texture = _pDevice->createTextureFromFile(L"Textures/stone01.dds");

	Entity* entity = createEntity();

	RenderData* data = new RenderData();
	data->addConstantBuffer(constantBuffer);
	data->setVertexBuffer(vb);
	data->setIndexBuffer(ib);

	RenderPass *renderPass1 = new RenderPass();
	renderPass1->init(vertexShader, nullptr, nullptr, nullptr, pixelShader);

	data->addRenderPass(renderPass1);

	data->setInputLayout(inputLayout);
	data->addTexture(texture);
	IRenderer* render = new Renderer(_pDevice);

	MeshComponent* grComponent = new MeshComponent(data, render);

	entity->add(grComponent);

	return entity;
}
