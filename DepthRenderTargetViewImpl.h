#pragma once
#include "ResourcesImpl.h"

class DepthRenderTargetViewImpl
{
public:
	DepthRenderTargetViewImpl() :
		Texture(nullptr) {}
	DepthRenderTargetViewImpl(TextureImpl* texture) :
		Texture(texture) {}

	TextureImpl* Texture;
};
