#include "stdafx.h"
#include "Skybox.h"


Skybox::Skybox(Device *aDevice, Entity *entity, Camera *aCamera)
	:EntityDecorator(entity),
	_pDevice(aDevice),
	_pCamera(aCamera)
{
	_pShaderFactory = new ShaderFactory(_pDevice);

	_renderData = EntityDecorator::getMeshComponent()->getRenderData();

	IRenderer* render = new SkyboxRenderer(_pDevice);

	EntityDecorator::getMeshComponent()->setRenderer(render);

	_renderData->dropRenderPass(0);

	Shader *vertexShader = _pShaderFactory->getShader(L"skyboxVS.hlsl", sky::EShaderType::STVertex);
	Shader *pixelShader = _pShaderFactory->getShader(L"skyboxPS.hlsl", sky::EShaderType::STPixel);

	RenderPass *renderPass1 = new RenderPass();
	renderPass1->init(vertexShader, nullptr, nullptr, nullptr, pixelShader);

	_renderData->addRenderPass(renderPass1);

	Texture *texture = _pDevice->createTextureFromFile(L"Textures/skymap.dds");//sky_cube.dds

	_renderData->setTexture(texture, 0);

	//createSphere(10, 10);
}


Skybox::~Skybox()
{
}

void Skybox::update()
{
	_renderData->setScale(XMFLOAT3(5, 5, 5));

	XMFLOAT3 eye = _pCamera->getPosition();

	_renderData->setTranslate(XMFLOAT3(eye.x, eye.y, eye.z));

	_renderData->updateTransform();

	EntityDecorator::update();
}

void Skybox::createSphere(int LatLines, int LongLines)
{
	int NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	int NumSphereFaces = ((LatLines - 3)*(LongLines)* 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		XMMATRIX Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			XMMATRIX Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;

	VertexBuffer* vertexBuffer = _pDevice->createVertexBuffer(vertices.data(), vertices.size(), sizeof(Vertex));

	_renderData->setVertexBuffer(vertexBuffer);

	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i*LongLines + j + 1;
			indices[k + 1] = i*LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i*LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	IndexBuffer *indexBuffer = _pDevice->createIndexBuffer(indices.data(), indices.size(), sizeof(DWORD), INDEX_BUFFER_FORMAT::IB_FORMAT_32);

	_renderData->setIndexBuffer(indexBuffer);
}
