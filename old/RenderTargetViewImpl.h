#pragma once
#include "ResourcesImpl.h"

class RenderTargetViewImpl
{
public:
	RenderTargetViewImpl():
		Texture(nullptr),
		MipIndex(0),
		ArraySliceIndex(-1)
	{}

	RenderTargetViewImpl(const RenderTargetViewImpl& Other) :
		Texture(Other.Texture),
		MipIndex(Other.MipIndex),
		ArraySliceIndex(Other.ArraySliceIndex)
	{}

	RenderTargetViewImpl(TextureImpl* InTexture) :
		Texture(InTexture),
		MipIndex(0),
		ArraySliceIndex(-1)
	{}

	RenderTargetViewImpl(TextureImpl* InTexture, unsigned int InMipIndex, unsigned int InArraySliceIndex) :
		Texture(InTexture),
		MipIndex(InMipIndex),
		ArraySliceIndex(InArraySliceIndex)
	{}

	TextureImpl* Texture;
	unsigned int MipIndex;
	unsigned int ArraySliceIndex;
};
