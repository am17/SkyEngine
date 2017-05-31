#pragma once
#include "Shader.h"
#include "RenderTarget.h"

class RenderPass
{
public:
	RenderPass(Shader *aVertexShader, Shader *aPixelShader, ERenderTarget aRenderTarget = ERenderTarget::RT_BACK_BUFFER)
		:pVertexShader(aVertexShader),
		pPixelShader(aPixelShader), 
		_renderTarget(aRenderTarget)
	{}
	~RenderPass(){}
	void setVertexShader(Shader* aVertexShader)
	{
		pVertexShader = aVertexShader;
	}
	Shader* getVertexShader() const
	{
		return pVertexShader;
	}
	void setPixelShader(Shader* aPixelShader)
	{
		pPixelShader = aPixelShader;
	}
	Shader* getPixelShader() const
	{
		return pPixelShader;
	}
	ERenderTarget getRenderTarget() const
	{
		return _renderTarget;
	}
private:
	Shader *pVertexShader;
	Shader *pPixelShader;
	ERenderTarget _renderTarget;
};

enum class Cull
{
	Back, 
	Front,
	Off
};

enum class ZTest
{
	Less, 
	Greater, 
	LEqual, 
	GEqual, 
	Equal, 
	NotEqual, 
	Always
};

enum class ZWrite
{
	On,
	Off
};