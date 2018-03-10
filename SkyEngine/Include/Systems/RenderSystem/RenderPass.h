#pragma once
#include "Resource.h"
#include "Shader.h"
#include <memory>

using namespace std;

class RenderPass
{
public:
	RenderPass();
	~RenderPass();
	void Bind();
private:
	shared_ptr<RasterizerState> RasterizerState;
	shared_ptr<DepthStencilState> DepthStencilState;
	shared_ptr<BlendState> BlendState;

	shared_ptr<VertexShader> VertexShader;
	shared_ptr<HullShader> HullShader;
	shared_ptr<DomainShader> DomainShader;
	shared_ptr<GeometryShader> GeometryShader;
	shared_ptr<PixelShader> PixelShader;
};

