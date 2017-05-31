#pragma once
#include <d3d11.h>
#include "ShaderTypes.h" 

using namespace std;

namespace sky
{
	

	struct D3D11VertexBufferState
	{
		ID3D11Buffer* VertexBuffer;
		unsigned int Stride;
		unsigned int Offset;
	};

	struct D3D11ConstantBufferState
	{
		ID3D11Buffer* Buffer;
		unsigned int FirstConstant;
		unsigned int NumConstants;
	};

	class D3D11StateCache
	{
	public:
		D3D11StateCache();
		virtual ~D3D11StateCache();
		void init(ID3D11DeviceContext* InDeviceContext, bool bInAlwaysSetIndexBuffers = false);
		void setShaderResourceView(EShaderType ShaderType, ID3D11ShaderResourceView* SRV, unsigned ResourceIndex);
		void getShaderResourceViews(EShaderType ShaderType, unsigned StartResourceIndex, unsigned NumResources, ID3D11ShaderResourceView** SRV);
		void setViewport(const D3D11_VIEWPORT Viewport);
		void setViewports(unsigned Count, D3D11_VIEWPORT* Viewports);
		void getViewport(D3D11_VIEWPORT* Viewport) const;
		void getViewports(unsigned* Count, D3D11_VIEWPORT* Viewports) const;
		void setSamplerState(EShaderType ShaderType, ID3D11SamplerState* SamplerState, unsigned SamplerIndex);
		void getSamplerState(EShaderType ShaderType, unsigned StartSamplerIndex, unsigned NumSamplerIndexes, ID3D11SamplerState** SamplerStates);
		void setConstantBuffer(EShaderType ShaderType, ID3D11Buffer* ConstantBuffer, unsigned SlotIndex);
		void getConstantBuffers(EShaderType ShaderType, unsigned StartSlotIndex, unsigned NumBuffers, ID3D11Buffer** ConstantBuffers);
		void setRasterizerState(ID3D11RasterizerState* State);
		void getRasterizerState(ID3D11RasterizerState** RasterizerState) const;
		void setBlendState(ID3D11BlendState* State, const float BlendFactor[4], unsigned int SampleMask);
		void getBlendState(ID3D11BlendState** BlendState, float BlendFactor[4], unsigned int* SampleMask) const;
		void setDepthStencilState(ID3D11DepthStencilState* State, unsigned int RefStencil);
		void getDepthStencilState(ID3D11DepthStencilState** DepthStencilState, unsigned int* StencilRef) const;
		void setVertexShader(ID3D11VertexShader* Shader);
		void getVertexShader(ID3D11VertexShader** VertexShader) const;
		void setHullShader(ID3D11HullShader* Shader);
		void getHullShader(ID3D11HullShader** HullShader) const;
		void setDomainShader(ID3D11DomainShader* Shader);
		void getDomainShader(ID3D11DomainShader** DomainShader) const;
		void setGeometryShader(ID3D11GeometryShader* Shader);
		void getGeometryShader(ID3D11GeometryShader** GeometryShader) const;
		void setPixelShader(ID3D11PixelShader* Shader);
		void getPixelShader(ID3D11PixelShader** PixelShader) const;
		void setComputeShader(ID3D11ComputeShader* Shader);
		void getComputeShader(ID3D11ComputeShader** ComputeShader) const;
		void setInputLayout(ID3D11InputLayout* InputLayout);
		void getInputLayout(ID3D11InputLayout** InputLayout) const;
		void setVertexBuffer(ID3D11Buffer* VertexBuffer, unsigned StreamIndex, unsigned Stride, unsigned Offset);
		void getVertexBuffer(unsigned StartStreamIndex, unsigned NumStreams, ID3D11Buffer** VertexBuffers, unsigned* Strides, unsigned* Offsets);
		void setIndexBuffer(ID3D11Buffer* IndexBuffer, DXGI_FORMAT Format, unsigned Offset);
		void getIndexBuffer(ID3D11Buffer** IndexBuffer, DXGI_FORMAT* Format, unsigned* Offset) const;
		void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology);
		void getPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY* PrimitiveTopology) const;
		void setContext(ID3D11DeviceContext* InDeviceContext);
		void ClearState();
	private:
		ID3D11DeviceContext* m_deviceContext;
		ID3D11ShaderResourceView* m_currentShaderResourceViews[6][D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		ID3D11RasterizerState* m_currentRasterizerState;

		unsigned int m_currentReferenceStencil;
		ID3D11DepthStencilState* m_currentDepthStencilState;
		ID3D11VertexShader* m_currentVertexShader;
		ID3D11HullShader* m_currentHullShader;
		ID3D11DomainShader* m_currentDomainShader;
		ID3D11GeometryShader* m_currentGeometryShader;
		ID3D11PixelShader* m_currentPixelShader;
		ID3D11ComputeShader* m_currentComputeShader;
		float m_currentBlendFactor[4];
		unsigned int m_currentBlendSampleMask;
		ID3D11BlendState* m_currentBlendState;
		unsigned int m_currentNumberOfViewports;
		D3D11_VIEWPORT m_currentViewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

		D3D11VertexBufferState m_currentVertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];

		ID3D11Buffer* m_currentIndexBuffer;
		DXGI_FORMAT m_currentIndexFormat;
		unsigned int m_currentIndexOffset;

		D3D11_PRIMITIVE_TOPOLOGY m_currentPrimitiveTopology;
		ID3D11InputLayout* m_currentInputLayout;
		ID3D11SamplerState* m_currentSamplerStates[6][D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];

		D3D11ConstantBufferState m_currentConstantBuffers[6][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];

		bool bAlwaysSetIndexBuffers;
	};
}

