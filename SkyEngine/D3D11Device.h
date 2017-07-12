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
private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mD3DDeviceIMContext;
	Microsoft::WRL::ComPtr<ID3D11Device> mDirect3DDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory1> mDXGIFactory;
	D3D_FEATURE_LEVEL FeatureLevel;
};

