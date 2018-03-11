#pragma once
#include <wrl/client.h>
#include "d3d11.h"
#include "../Texture2D.h"
#include "D3D11Device.h"

class D3D11Texture2D : public Texture2D
{
public:
	D3D11Texture2D(D3D11Device *device);
	~D3D11Texture2D();
	bool IsMultisampled() const override;
	int GetWidth() const override;
	int GetHeight() const override;
	bool Create(int width, int height, bool isMultisampled = false) override;
	bool Create(ID3D11Texture2D* texture);
	Microsoft::WRL::ComPtr<ID3D11Texture2D>  Resource;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  RenderTargetView;
private:
	D3D11Device *pDevice;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_shaderResourceView;
	bool Multisampled;
	int Width;
	int Height;
};
