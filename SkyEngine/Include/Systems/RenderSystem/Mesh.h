#pragma once
#include "Systems\RenderSystem\VertexBuffer.h"
#include "Systems\RenderSystem\IndexBuffer.h"
#include <memory>

class Mesh
{
public:
	Mesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);
	~Mesh();
private:
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
};