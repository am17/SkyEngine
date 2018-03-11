#include "Systems\RenderSystem\D3D11\D3D11Texture2D.h"

D3D11Texture2D::D3D11Texture2D(D3D11Device *device):
	pDevice(device),
	Multisampled(false),
	Width(0),
	Height(0)
{
}

D3D11Texture2D::~D3D11Texture2D()
{
}

bool D3D11Texture2D::IsMultisampled() const
{
	return Multisampled;
}

int D3D11Texture2D::GetWidth() const
{
	return Width;
}

int D3D11Texture2D::GetHeight() const
{
	return Height;
}

bool D3D11Texture2D::Create(int width, int height, const void * pData, bool createRTV, bool createDSV, unsigned int multiSampleCount, unsigned int multiSampleQuality)
{
	assert(width > 0 && height > 0);
	assert(createRTV != createDSV);

	Width = width;
	Height = height;
	Multisampled = multiSampleCount > 0;

	if (createRTV && Resource)
	{
		CreateRenderTarget(Resource.Get());
	}

	return true;
}

bool D3D11Texture2D::CreateRenderTarget(ID3D11Texture2D * texture)
{
	Resource = texture;

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	Width = desc.Width;
	Height = desc.Height;

	//nullptr for all subresorces in 0 mipmap level
	HRESULT result = pDevice->GetDevice()->CreateRenderTargetView(Resource.Get(), nullptr, RenderTargetView.ReleaseAndGetAddressOf());

	return SUCCEEDED(result);
}

