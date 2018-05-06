#include "Systems\RenderSystem\D3D11\D3D11Texture2D.h"

D3D11Texture2D::D3D11Texture2D(int width, int height, bool multisampled):
	Multisampled(multisampled),
	Width(width),
	Height(height)
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

