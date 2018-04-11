#include "../Include/Systems/RenderSystem/Mesh.h"

Mesh::Mesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
	:vertexBuffer(vertexBuffer),
	indexBuffer(indexBuffer)
{
}

Mesh::~Mesh()
{
}
