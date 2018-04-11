#include "../Include/Systems/RenderSystem/MeshFactory.h"
#include "../Include/Systems/RenderSystem/Resource.h"

MeshFactory::MeshFactory(IDeviceImpl* device)
	:pDevice(device)
{
}

MeshFactory::~MeshFactory()
{
}

Mesh * MeshFactory::CreateTriangle()
{
	VertexType* vertices = new VertexType[3];

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	VertexBuffer* vBuffer = pDevice->CreateVertexBuffer(vertices, 3, sizeof(VertexType), BUFFER_USAGE_FLAGS::STATIC);

	unsigned long* indices = new unsigned long[3];
	indices[0] = 0; 
	indices[1] = 1;
	indices[2] = 2; 

	IndexBuffer* iBuffer = pDevice->CreateIndexBuffer(indices, 3, sizeof(unsigned long), BUFFER_USAGE_FLAGS::STATIC);

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return new Mesh(vBuffer, iBuffer);
}
