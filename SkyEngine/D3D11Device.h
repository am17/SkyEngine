#pragma once
#include "IDeviceImpl.h"
#include "d3d11.h"
#include <memory>
#include <wrl/client.h>
#include "common.h"

class SKYENGINEDLL D3D11Device : public IDeviceImpl
{
public:
	D3D11Device();
	virtual ~D3D11Device();
	bool Init() override;
	void *CreateTexture2D(unsigned int width, unsigned int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality) override;
	ID3D11Device *GetDevice() const { return mDirect3DDevice.Get();  }
	IDXGIFactory1 *GetFactory() const { return mDXGIFactory.Get(); }
	VertexShader* CreateVertexShader(const void* pByteCode, size_t ByteCodeLength) override;
	HullShader* CreateHullShader(const void* pByteCode, size_t ByteCodeLength) override;
	DomainShader* CreateDomainShader(const void* pByteCode, size_t ByteCodeLength) override;
	PixelShader* CreatePixelShader(const void* pByteCode, size_t ByteCodeLength) override;
	GeometryShader* CreateGeometryShader(const void* pByteCode, size_t ByteCodeLength) override;
	ComputeShader* CreateComputeShader(const void* pByteCode, size_t ByteCodeLength) override;
private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mD3DDeviceIMContext;
	Microsoft::WRL::ComPtr<ID3D11Device> mDirect3DDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory1> mDXGIFactory;
	D3D_FEATURE_LEVEL FeatureLevel;
};

