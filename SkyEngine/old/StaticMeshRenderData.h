#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IShader.h"
#include "MeshRenderData.h"

namespace sky
{
	class StaticMeshResources
	{
	public:
		StaticMeshResources(StaticMeshVertexBuffer* vertex_buffer, StaticIndexBuffer *index_buffer);
		~StaticMeshResources();
		int getNumVertices() const;
		StaticMeshVertexBuffer *getVertexBuffer() const;
		StaticIndexBuffer *getIndexBuffer() const;
	private:
		StaticMeshVertexBuffer *VertexBuffer;
		StaticIndexBuffer *StaticIndicesBuffer;
	};

	class StaticMeshRenderData : public MeshRenderData
	{
	public:
		StaticMeshRenderData();
		~StaticMeshRenderData();
		void setResources(StaticMeshResources *resources, IVertexShader *vertexShader, IPixelShader *pixelShader);
		StaticMeshResources *getResources() const;
	private:
		StaticMeshResources *m_resources;
	};
}

