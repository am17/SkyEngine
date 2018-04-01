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

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = multiSampleCount;
	depthStencilDesc.SampleDesc.Quality = multiSampleQuality;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (createRTV)
	{
		//tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		depthStencilDesc.BindFlags = D3D11_BIND_RENDER_TARGET;;
	}

	if (createDSV)
	{
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}

	pDevice->GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, Resource.GetAddressOf());

	if (createRTV && Resource)
	{
		CreateRenderTarget(Resource.Get());
	}

	if (createDSV && Resource)
	{
		CreateDepthStencil(Resource.Get());
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

bool D3D11Texture2D::CreateDepthStencil(ID3D11Texture2D * texture)
{
	Resource = texture;

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	//nullptr for all subresorces in 0 mipmap level
	HRESULT result = pDevice->GetDevice()->CreateDepthStencilView(Resource.Get(), &depthStencilViewDesc, DepthStencilView.ReleaseAndGetAddressOf());

	return SUCCEEDED(result);
}

