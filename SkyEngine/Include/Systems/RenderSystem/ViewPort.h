#pragma once
#include "Resource.h"
#include <memory>
#include "Texture2D.h"

using namespace std;

class ViewPort : public Resource
{
public:
	ViewPort() {}
	~ViewPort() {}
	virtual void Present() {}
	virtual void Clear() {}
	Texture2D* RenderTargetTexture;
	Texture2D* DepthStencilTexture;
};