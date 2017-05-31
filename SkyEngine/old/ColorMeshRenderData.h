#pragma once
#include "MeshRenderData.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace sky
{
	class ColorMeshResources
	{
	public:
		ColorMeshResources(ColorVertexBuffer* vertex_buffer, StaticIndexBuffer *index_buffer);
		~ColorMeshResources();
		int getNumVertices() const;
		ColorVertexBuffer *getVertexBuffer() const;
		StaticIndexBuffer *getIndexBuffer() const;
	private:
		ColorVertexBuffer *VertexBuffer;
		StaticIndexBuffer *StaticIndicesBuffer;
	};

	class ColorMeshRenderData : public MeshRenderData
	{
	public:
		ColorMeshRenderData();
		~ColorMeshRenderData();
		void setResources(ColorMeshResources *resources, IVertexShader *vertexShader, IPixelShader *pixelShader);
		ColorMeshResources *getResources() const;
	private:
		ColorMeshResources *m_resources;
	};
}

