#pragma once
#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;
};