#pragma once
#include "D3D11PipelineState.h"
#include "D3D11StateCache.h"

namespace sky
{
	class MeshRenderData
	{
	public:
		MeshRenderData()
			:pipelineState(nullptr)
		{
			pipelineState = new D3D11PipelineState();
		}
		virtual ~MeshRenderData()
		{
			if (pipelineState)
			{
				delete pipelineState;
				pipelineState = nullptr;
			}
		}
		void initStagesData(IAPIBuffer *vertexBuffer, IAPIBuffer *indexBuffer, IVertexShader *vertexShader, IPixelShader *pixelShader)
		{
			IAStage *iaStage = pipelineState->getIAStageData();
			VSStage *vsStage = pipelineState->getVSStageData();
			PSStage *psStage = pipelineState->getPSStageData();

			VertexInputLayout* layout = vertexShader->getInputSignature();

			iaStage->setVertexBufferCount(vertexShader->getVertexBufferCount());

			iaStage->setVertexBuffer(vertexBuffer, 0);

			iaStage->setIndexBuffer(indexBuffer);

			iaStage->setInputLayout(layout);

			vsStage->setVertexShader(vertexShader);

			psStage->setPixelShader(pixelShader);

			pipelineState->setReady(true);
		}
		D3D11PipelineState *getPipelineState() const
		{
			return pipelineState;
		}
		template<EShaderType shaderType>
		void updateConstantBuffer(unsigned int bufferIndex, void* data) const
		{
			ConstantBuffer *constantBuffer = nullptr;

			switch (shaderType)
			{
			case sky::EShaderType::ST_Vertex:
				constantBuffer = pipelineState->getVSStageData()->getConstantBuffer(bufferIndex);
				assert(constantBuffer != nullptr);
				constantBuffer->setData(data);
				break;
			case sky::EShaderType::ST_Hull:
				break;
			case sky::EShaderType::ST_Domain:
				break;
			case sky::EShaderType::ST_Pixel:
				constantBuffer = pipelineState->getPSStageData()->getConstantBuffer(bufferIndex);
				assert(constantBuffer != nullptr);
				constantBuffer->setData(data);
				break;
			case sky::EShaderType::ST_Geometry:
				break;
			case sky::EShaderType::ST_Compute:
				break;
			default:
				break;
			}
		}
		void setBackfaceCulling(bool cullBackface) const
		{
			RSStage *rsState = pipelineState->getRSStageData();

			rsState->setBackfaceCulling(cullBackface);
		}
		void setZBufferEnabled(bool zBufferEnabled) const
		{
			OMStage *omState = pipelineState->getOMStageData();

			omState->setZBufferEnabled(zBufferEnabled);
		}
		void setPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
			IAStage *iaStage = pipelineState->getIAStageData();

			iaStage->setPrimitiveTopology(topology);
		}
	protected:
		D3D11PipelineState *pipelineState;
	};
}
