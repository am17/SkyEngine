#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "../Texture2D.h"
#include "D3D11Device.h"

class D3D11Texture2D : public Texture2D
{
public:
	D3D11Texture2D(int width, int height, bool multisampled = false);
	~D3D11Texture2D();
	bool IsMultisampled() const override;
	int GetWidth() const override;
	int GetHeight() const override;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>  Resource;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  ShaderResourceView;
private:
	bool Multisampled;
	int Width;
	int Height;
};
