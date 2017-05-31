#include "stdafx.h"
#include "ColorMeshRenderData.h"

namespace sky
{
	ColorMeshResources::ColorMeshResources(ColorVertexBuffer* vertex_buffer, StaticIndexBuffer* index_buffer)
		:VertexBuffer(vertex_buffer),
		StaticIndicesBuffer(index_buffer)
	{
	}

	ColorMeshResources::~ColorMeshResources()
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

	int ColorMeshResources::getNumVertices() const
	{
		return VertexBuffer->getVerticesCount();
	}

	ColorVertexBuffer* ColorMeshResources::getVertexBuffer() const
	{
		return VertexBuffer;
	}

	StaticIndexBuffer* ColorMeshResources::getIndexBuffer() const
	{
		return StaticIndicesBuffer;
	}

	ColorMeshRenderData::ColorMeshRenderData()
		: MeshRenderData(), 
		m_resources(nullptr)
	{
		IAStage *iaStage = pipelineState->getIAStageData();

		iaStage->setPrimitiveTopology(PRIMITIVE_TOPOLOGY::LINELIST);

		pipelineState->setReady(true);
	}


	ColorMeshRenderData::~ColorMeshRenderData()
	{
		if (m_resources)
		{
			delete m_resources;
			m_resources = nullptr;
		}
	}

	void ColorMeshRenderData::setResources(ColorMeshResources* resources, IVertexShader* vertexShader, IPixelShader* pixelShader)
	{
		assert(resources != nullptr && vertexShader != nullptr && pixelShader != nullptr);

		m_resources = resources;

		ColorVertexBuffer *vertex_buffer = m_resources->getVertexBuffer();

		IAPIBuffer *vertexBuffer = vertex_buffer->getAPIVertexBuffer();

		StaticIndexBuffer *index_buffer = m_resources->getIndexBuffer();

		IAPIBuffer *indexBuffer = index_buffer->getAPIIndexBuffer();

		initStagesData(vertexBuffer, indexBuffer, vertexShader, pixelShader);
	}

	ColorMeshResources* ColorMeshRenderData::getResources() const
	{
		return m_resources;
	}
}
