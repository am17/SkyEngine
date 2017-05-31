#include "stdafx.h"
#include "StaticMeshRenderData.h"
#include "globals.h"
#include "ConstantBufferFormats.h"

namespace sky
{
	StaticMeshResources::StaticMeshResources(StaticMeshVertexBuffer* vertex_buffer, StaticIndexBuffer *index_buffer)
		:VertexBuffer(vertex_buffer),
		StaticIndicesBuffer(index_buffer)
	{

	}

	StaticMeshResources::~StaticMeshResources()
	{
		if (VertexBuffer)
		{
			delete VertexBuffer;
			VertexBuffer = nullptr;
		}

		if (StaticIndicesBuffer)
		{
			delete StaticIndicesBuffer;
			StaticIndicesBuffer = nullptr;
		}
	}

	int StaticMeshResources::getNumVertices() const
	{
		return VertexBuffer->getVerticesCount();
	}

	StaticMeshVertexBuffer* StaticMeshResources::getVertexBuffer() const
	{
		return VertexBuffer;
	}

	StaticIndexBuffer* StaticMeshResources::getIndexBuffer() const
	{
		return StaticIndicesBuffer;
	}

	StaticMeshRenderData::StaticMeshRenderData()
		: MeshRenderData(),
		m_resources(nullptr)
	{
		IAStage *iaStage = pipelineState->getIAStageData();

		iaStage->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::TRIANGLELIST);

		pipelineState->setReady(true);
	}

	StaticMeshRenderData::~StaticMeshRenderData()
	{
		if (m_resources)
		{
			delete m_resources;
			m_resources = nullptr;
		}
	}

	void StaticMeshRenderData::setResources(StaticMeshResources *resources, IVertexShader *vertexShader, IPixelShader *pixelShader)
	{
		assert(resources != nullptr && vertexShader != nullptr && pixelShader != nullptr);

		m_resources = resources;

		StaticMeshVertexBuffer *vertex_buffer = m_resources->getVertexBuffer();

		IAPIBuffer *vertexBuffer = vertex_buffer->getAPIVertexBuffer();

		StaticIndexBuffer *index_buffer = m_resources->getIndexBuffer();

		IAPIBuffer *indexBuffer = index_buffer->getAPIIndexBuffer();

		initStagesData(vertexBuffer, indexBuffer, vertexShader, pixelShader);
	}


	StaticMeshResources* StaticMeshRenderData::getResources() const
	{
		return m_resources;
	}
}
