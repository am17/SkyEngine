#include "stdafx.h"
#include "D3D11StateCache.h"

namespace sky
{
	D3D11StateCache::D3D11StateCache()
		:m_deviceContext(nullptr), 
		m_currentRasterizerState(nullptr), 
		m_currentReferenceStencil(0), 
		m_currentDepthStencilState(nullptr), 
		m_currentVertexShader(nullptr), 
		m_currentHullShader(nullptr), 
		m_currentDomainShader(nullptr), 
		m_currentGeometryShader(nullptr), 
		m_currentPixelShader(nullptr), 
		m_currentComputeShader(nullptr), 
		m_currentBlendSampleMask(0), 
		m_currentBlendState(nullptr), 
		m_currentNumberOfViewports(0), 
		m_currentIndexBuffer(nullptr), 
		m_currentIndexFormat(), 
		m_currentIndexOffset(0), 
		m_currentPrimitiveTopology(),
		m_currentInputLayout(nullptr), 
		bAlwaysSetIndexBuffers(false)
	{
		ZeroMemory(&m_currentShaderResourceViews, sizeof(m_currentShaderResourceViews));
	}

	D3D11StateCache::~D3D11StateCache()
	{
	}

	void D3D11StateCache::init(ID3D11DeviceContext* InDeviceContext, bool bInAlwaysSetIndexBuffers)
	{
		setContext(InDeviceContext);

		bAlwaysSetIndexBuffers = bInAlwaysSetIndexBuffers;
	}

	void D3D11StateCache::setShaderResourceView(EShaderType ShaderType, ID3D11ShaderResourceView* SRV, unsigned int ResourceIndex)
	{
		int shaderTypeIndex = static_cast<int>(ShaderType);

		if (ResourceIndex < ARRAYSIZE(m_currentShaderResourceViews[shaderTypeIndex]))
		{
			if ((m_currentShaderResourceViews[shaderTypeIndex][ResourceIndex] != SRV))
			{
				if (SRV)
				{
					SRV->AddRef();
				}
				if (m_currentShaderResourceViews[shaderTypeIndex][ResourceIndex])
				{
					m_currentShaderResourceViews[shaderTypeIndex][ResourceIndex]->Release();
				}
				m_currentShaderResourceViews[shaderTypeIndex][ResourceIndex] = SRV;

				switch (ShaderType)
				{
				case EShaderType::ST_Vertex:
					m_deviceContext->VSSetShaderResources(ResourceIndex, 1, &SRV); break;
				case EShaderType::ST_Hull:
					m_deviceContext->HSSetShaderResources(ResourceIndex, 1, &SRV); break;
				case EShaderType::ST_Domain:
					m_deviceContext->DSSetShaderResources(ResourceIndex, 1, &SRV); break;
				case EShaderType::ST_Geometry:
					m_deviceContext->GSSetShaderResources(ResourceIndex, 1, &SRV); break;
				case EShaderType::ST_Pixel:
					m_deviceContext->PSSetShaderResources(ResourceIndex, 1, &SRV); break;
				case EShaderType::ST_Compute:
					m_deviceContext->CSSetShaderResources(ResourceIndex, 1, &SRV); break;
				default:
					break;
				}
			}
		}
	}

	void D3D11StateCache::getShaderResourceViews(EShaderType ShaderType, unsigned int StartResourceIndex, unsigned int NumResources, ID3D11ShaderResourceView** SRV)
	{
		if (StartResourceIndex + NumResources <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
		{
			int shaderTypeIndex = static_cast<int>(ShaderType);

			for (unsigned int ResourceLoop = 0; ResourceLoop < NumResources; ResourceLoop++)
			{
				SRV[ResourceLoop] = m_currentShaderResourceViews[shaderTypeIndex][ResourceLoop + StartResourceIndex];
				if (SRV[ResourceLoop])
				{
					SRV[ResourceLoop]->AddRef();
				}
			}
		}
	}

	void D3D11StateCache::setViewport(const D3D11_VIEWPORT Viewport)
	{
		if (m_currentNumberOfViewports != 1)
		{
			memcpy_s(&m_currentViewport[0], sizeof(D3D11_VIEWPORT), &Viewport, sizeof(D3D11_VIEWPORT));
			m_currentNumberOfViewports = 1;
			m_deviceContext->RSSetViewports(1, &Viewport);
		}

	}

	void D3D11StateCache::setViewports(unsigned int Count, D3D11_VIEWPORT* Viewports)
	{
		if (m_currentNumberOfViewports != Count)
		{
			memcpy_s(&m_currentViewport[0], sizeof(D3D11_VIEWPORT) * Count, Viewports, sizeof(D3D11_VIEWPORT) * Count);
			m_currentNumberOfViewports = Count;
			m_deviceContext->RSSetViewports(Count, Viewports);
		}

	}

	void D3D11StateCache::getViewport(D3D11_VIEWPORT *Viewport) const
	{
		if (Viewport)
		{
			memcpy_s(Viewport, sizeof(D3D11_VIEWPORT), &m_currentViewport, sizeof(D3D11_VIEWPORT));
		}
	}

	void D3D11StateCache::getViewports(unsigned int* Count, D3D11_VIEWPORT *Viewports) const
	{
		if (*Count && Viewports)
		{
			unsigned int StorageSizeCount = static_cast<unsigned int>(*Count);
			unsigned int CopyCount = min(min(StorageSizeCount, (unsigned int)m_currentNumberOfViewports), D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE);
			if (CopyCount > 0)
			{
				memcpy_s(Viewports, sizeof(D3D11_VIEWPORT) * CopyCount, &m_currentViewport[0], sizeof(D3D11_VIEWPORT) * CopyCount);
			}

			if (StorageSizeCount > CopyCount)
			{
				memset(&Viewports[CopyCount], 0, sizeof(D3D11_VIEWPORT) * (StorageSizeCount - CopyCount));
			}

			*Count = m_currentNumberOfViewports;
		}
	}

	void D3D11StateCache::setSamplerState(EShaderType ShaderType, ID3D11SamplerState* SamplerState, unsigned int SamplerIndex)
	{
		int shaderTypeIndex = static_cast<int>(ShaderType);

		if (SamplerIndex < ARRAYSIZE(m_currentSamplerStates[shaderTypeIndex]))
		{
			if (m_currentSamplerStates[shaderTypeIndex][SamplerIndex] != SamplerState)
			{
				m_currentSamplerStates[shaderTypeIndex][SamplerIndex] = SamplerState;

				switch (ShaderType)
				{
				case EShaderType::ST_Vertex:
					m_deviceContext->VSSetSamplers(SamplerIndex, 1, &SamplerState); break;
				case EShaderType::ST_Hull:
					m_deviceContext->HSSetSamplers(SamplerIndex, 1, &SamplerState); break;
				case EShaderType::ST_Domain:
					m_deviceContext->DSSetSamplers(SamplerIndex, 1, &SamplerState); break;
				case EShaderType::ST_Geometry:
					m_deviceContext->GSSetSamplers(SamplerIndex, 1, &SamplerState); break;
				case EShaderType::ST_Pixel:
					m_deviceContext->PSSetSamplers(SamplerIndex, 1, &SamplerState); break;
				case EShaderType::ST_Compute:
					m_deviceContext->CSSetSamplers(SamplerIndex, 1, &SamplerState); break;
				default:
					break;
				}
			}
		}
	}

	void D3D11StateCache::getSamplerState(EShaderType ShaderType, unsigned int StartSamplerIndex, unsigned int NumSamplerIndexes, ID3D11SamplerState** SamplerStates)
	{
		if (StartSamplerIndex + NumSamplerIndexes <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
		{
			int shaderTypeIndex = static_cast<int>(ShaderType);

			for (unsigned int StateLoop = 0; StateLoop < NumSamplerIndexes; StateLoop++)
			{
				SamplerStates[StateLoop] = reinterpret_cast<ID3D11SamplerState*>(m_currentShaderResourceViews[shaderTypeIndex][StateLoop + StartSamplerIndex]);
				if (SamplerStates[StateLoop])
				{
					SamplerStates[StateLoop]->AddRef();
				}
			}
		}
	}

	void D3D11StateCache::setConstantBuffer(EShaderType ShaderType, ID3D11Buffer* ConstantBuffer, unsigned int SlotIndex)
	{
		int shaderTypeIndex = static_cast<int>(ShaderType);

		D3D11ConstantBufferState& current = m_currentConstantBuffers[shaderTypeIndex][SlotIndex];

		if ((current.Buffer != ConstantBuffer || current.FirstConstant != 0 || current.NumConstants != D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT))
		{
			current.Buffer = ConstantBuffer;
			current.FirstConstant = 0;
			current.NumConstants = D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT;

			switch (ShaderType)
			{
			case EShaderType::ST_Vertex:
				m_deviceContext->VSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
			case EShaderType::ST_Hull:
				m_deviceContext->HSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
			case EShaderType::ST_Domain:
				m_deviceContext->DSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
			case EShaderType::ST_Geometry:
				m_deviceContext->GSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
			case EShaderType::ST_Pixel:
				m_deviceContext->PSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
			case EShaderType::ST_Compute:
				m_deviceContext->CSSetConstantBuffers(SlotIndex, 1, &ConstantBuffer); break;
			default:
				break;
			}
		}
	}

	void D3D11StateCache::getConstantBuffers(EShaderType ShaderType, unsigned int StartSlotIndex, unsigned int NumBuffers, ID3D11Buffer** ConstantBuffers)
	{
		if (StartSlotIndex + NumBuffers <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
		{
			int shaderTypeIndex = static_cast<int>(ShaderType);

			for (unsigned int constantLoop = 0; constantLoop < NumBuffers; constantLoop++)
			{
				D3D11ConstantBufferState& cb = m_currentConstantBuffers[shaderTypeIndex][constantLoop + StartSlotIndex];
				ConstantBuffers[constantLoop] = cb.Buffer;
				if (ConstantBuffers[constantLoop])
				{
					ConstantBuffers[constantLoop]->AddRef();
				}
			}
		}
	}

	void D3D11StateCache::setRasterizerState(ID3D11RasterizerState* State)
	{
		if (m_currentRasterizerState != State)
		{
			m_currentRasterizerState = State;
			m_deviceContext->RSSetState(State);
		}
	}

	void D3D11StateCache::getRasterizerState(ID3D11RasterizerState** RasterizerState) const
	{
		*RasterizerState = m_currentRasterizerState;
		if (m_currentRasterizerState)
		{
			m_currentRasterizerState->AddRef();
		}
	}

	void D3D11StateCache::setBlendState(ID3D11BlendState* State, const float BlendFactor[4], unsigned int SampleMask)
	{
		if (m_currentBlendState != State || m_currentBlendSampleMask != SampleMask)
		{
			m_currentBlendState = State;
			m_currentBlendSampleMask = SampleMask;
			memcpy_s(m_currentBlendFactor, sizeof(m_currentBlendFactor), BlendFactor, sizeof(m_currentBlendFactor));
			m_deviceContext->OMSetBlendState(State, BlendFactor, SampleMask);
		}
	}

	void D3D11StateCache::getBlendState(ID3D11BlendState** BlendState, float BlendFactor[4], unsigned int* SampleMask) const
	{
		*BlendState = m_currentBlendState;
		if (m_currentBlendState)
		{
			m_currentBlendState->AddRef();
		}
		*SampleMask = m_currentBlendSampleMask;
		memcpy_s(BlendFactor, sizeof(m_currentBlendFactor), m_currentBlendFactor, sizeof(m_currentBlendFactor));
	}

	void D3D11StateCache::setDepthStencilState(ID3D11DepthStencilState* State, unsigned int RefStencil)
	{
		if (m_currentDepthStencilState != State || m_currentReferenceStencil != RefStencil)
		{
			m_currentDepthStencilState = State;
			m_currentReferenceStencil = RefStencil;
			m_deviceContext->OMSetDepthStencilState(State, RefStencil);
		}
	}

	void D3D11StateCache::getDepthStencilState(ID3D11DepthStencilState** DepthStencilState, unsigned int* StencilRef) const
	{
		*DepthStencilState = m_currentDepthStencilState;
		*StencilRef = m_currentReferenceStencil;
		if (m_currentDepthStencilState)
		{
			m_currentDepthStencilState->AddRef();
		}
	}

	void D3D11StateCache::setVertexShader(ID3D11VertexShader* Shader)
	{
		if ((m_currentVertexShader != Shader))
		{
			m_currentVertexShader = Shader;
			m_deviceContext->VSSetShader(Shader, nullptr, 0);
		}
	}


	void D3D11StateCache::getVertexShader(ID3D11VertexShader** VertexShader) const
	{
		*VertexShader = m_currentVertexShader;
		if (m_currentVertexShader)
		{
			m_currentVertexShader->AddRef();
		}
	}

	void D3D11StateCache::setHullShader(ID3D11HullShader* Shader)
	{
		if (m_currentHullShader != Shader)
		{
			m_currentHullShader = Shader;
			m_deviceContext->HSSetShader(Shader, nullptr, 0);
		}
	}


	void D3D11StateCache::getHullShader(ID3D11HullShader** HullShader) const
	{
		*HullShader = m_currentHullShader;
		if (m_currentHullShader)
		{
			m_currentHullShader->AddRef();
		}
	}

	void D3D11StateCache::setDomainShader(ID3D11DomainShader* Shader)
	{
		if ((m_currentDomainShader != Shader))
		{
			m_currentDomainShader = Shader;
			m_deviceContext->DSSetShader(Shader, nullptr, 0);
		}
	}

	void D3D11StateCache::getDomainShader(ID3D11DomainShader** DomainShader) const
	{
		*DomainShader = m_currentDomainShader;
		if (m_currentDomainShader)
		{
			m_currentDomainShader->AddRef();
		}
	}

	void D3D11StateCache::setGeometryShader(ID3D11GeometryShader* Shader)
	{
		if (m_currentGeometryShader != Shader)
		{
			m_currentGeometryShader = Shader;
			m_deviceContext->GSSetShader(Shader, nullptr, 0);
		}
	}

	void D3D11StateCache::getGeometryShader(ID3D11GeometryShader** GeometryShader) const
	{
		*GeometryShader = m_currentGeometryShader;
		if (m_currentGeometryShader)
		{
			m_currentGeometryShader->AddRef();
		}
	}

	void D3D11StateCache::setPixelShader(ID3D11PixelShader* Shader)
	{
		if (m_currentPixelShader != Shader)
		{
			m_currentPixelShader = Shader;
			m_deviceContext->PSSetShader(Shader, nullptr, 0);
		}
	}

	void D3D11StateCache::getPixelShader(ID3D11PixelShader** PixelShader) const
	{
		*PixelShader = m_currentPixelShader;
		if (m_currentPixelShader)
		{
			m_currentPixelShader->AddRef();
		}
	}

	void D3D11StateCache::setComputeShader(ID3D11ComputeShader* Shader)
	{
		if (m_currentComputeShader != Shader)
		{
			m_currentComputeShader = Shader;
			m_deviceContext->CSSetShader(Shader, nullptr, 0);
		}
	}

	void D3D11StateCache::getComputeShader(ID3D11ComputeShader** ComputeShader) const
	{
		*ComputeShader = m_currentComputeShader;
		if (m_currentComputeShader)
		{
			m_currentComputeShader->AddRef();
		}
	}

	void D3D11StateCache::setInputLayout(ID3D11InputLayout* InputLayout)
	{
		if (m_currentInputLayout != InputLayout)
		{
			m_currentInputLayout = InputLayout;
			m_deviceContext->IASetInputLayout(InputLayout);
		}
	}

	void D3D11StateCache::getInputLayout(ID3D11InputLayout** InputLayout) const
	{
		*InputLayout = m_currentInputLayout;
		if (m_currentInputLayout)
		{
			m_currentInputLayout->AddRef();
		}
	}

	void D3D11StateCache::setVertexBuffer(ID3D11Buffer* VertexBuffer, unsigned int StreamIndex, unsigned int Stride, unsigned int Offset)
	{
		if (StreamIndex < ARRAYSIZE(m_currentVertexBuffers))
		{
			D3D11VertexBufferState& Slot = m_currentVertexBuffers[StreamIndex];
			if ((Slot.VertexBuffer != VertexBuffer || Slot.Offset != Offset || Slot.Stride != Stride))
			{
				Slot.VertexBuffer = VertexBuffer;
				Slot.Offset = Offset;
				Slot.Stride = Stride;

				m_deviceContext->IASetVertexBuffers(StreamIndex, 1, &VertexBuffer, &Stride, &Offset); 
			}
		}
	}

	void D3D11StateCache::getVertexBuffer(unsigned int StartStreamIndex, unsigned int NumStreams, ID3D11Buffer** VertexBuffers, unsigned int* Strides, unsigned int* Offsets)
	{
		if (StartStreamIndex + NumStreams <= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT)
		{
			for (unsigned int StreamLoop = 0; StreamLoop < NumStreams; StreamLoop++)
			{
				D3D11VertexBufferState& Slot = m_currentVertexBuffers[StreamLoop + StartStreamIndex];
				VertexBuffers[StreamLoop] = Slot.VertexBuffer;
				Strides[StreamLoop] = Slot.Stride;
				Offsets[StreamLoop] = Slot.Offset;
				if (Slot.VertexBuffer)
				{
					Slot.VertexBuffer->AddRef();
				}
			}
		}
	}

	void D3D11StateCache::setIndexBuffer(ID3D11Buffer* IndexBuffer, DXGI_FORMAT Format, unsigned int Offset)
	{
		if (bAlwaysSetIndexBuffers || (m_currentIndexBuffer != IndexBuffer || m_currentIndexFormat != Format || m_currentIndexOffset != Offset))
		{
			m_currentIndexBuffer = IndexBuffer;
			m_currentIndexFormat = Format;
			m_currentIndexOffset = Offset;

			m_deviceContext->IASetIndexBuffer(IndexBuffer, Format, Offset);
		}
	}

	void D3D11StateCache::getIndexBuffer(ID3D11Buffer** IndexBuffer, DXGI_FORMAT* Format, unsigned int* Offset) const
	{
		*IndexBuffer = m_currentIndexBuffer;
		*Format = m_currentIndexFormat;
		*Offset = m_currentIndexOffset;
		if (m_currentIndexBuffer)
		{
			m_currentIndexBuffer->AddRef();
		}
	}

	void D3D11StateCache::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology)
	{
		if (m_currentPrimitiveTopology != PrimitiveTopology)
		{
			m_currentPrimitiveTopology = PrimitiveTopology;
			m_deviceContext->IASetPrimitiveTopology(PrimitiveTopology);
		}
	}

	void D3D11StateCache::getPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY* PrimitiveTopology) const
	{
		*PrimitiveTopology = m_currentPrimitiveTopology;
	}

	void D3D11StateCache::setContext(ID3D11DeviceContext* InDeviceContext)
	{
		m_deviceContext = InDeviceContext;
		ClearState();
	}

	void D3D11StateCache::ClearState() {}
}
