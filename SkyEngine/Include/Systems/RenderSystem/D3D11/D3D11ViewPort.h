#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "../ViewPort.h"
#include "D3D11Device.h"

class D3D11ViewPort : public ViewPort
{
public:
	D3D11ViewPort(D3D11Device *device, HWND hWnd, unsigned int backBufferWidth, unsigned int backBufferHeight, bool isFullscreen);
	~D3D11ViewPort();
	void Present() override;
	void Clear() override;
private:
	HWND WindowHandle;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	D3D11Device *pDevice;
	float Width;
	float Height;
	bool IsFullscreen;

	//
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
};
