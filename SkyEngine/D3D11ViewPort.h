#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "IDeviceImpl.h"
#include <memory>
#include "common.h"

using namespace std;

class SKYENGINEDLL D3D11ViewPort
{
public:
	D3D11ViewPort(IDeviceImpl *device, unsigned int width, unsigned int height);
	virtual ~D3D11ViewPort();
	bool Init(HWND ouputWindow, bool IsFullscreen = false);
	void Clear();
	void Present(int SyncInterval = 0);
	void RenderToBackBuffer();
private:
	shared_ptr<IDeviceImpl> mDevice;
	int mOutputWidth;
	int mOutputHeight;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  mDepthStencilView;
};

