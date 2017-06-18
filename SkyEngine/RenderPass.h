#pragma once
#include "Shader.h"

class RenderPass
{
public:
	RenderPass(): 
		_vertexShader(nullptr),
		_hullShader(nullptr),
		_domainShader(nullptr),
		_geometryShader(nullptr),
		_pixelShader(nullptr) {}
	virtual ~RenderPass() {}
	void apply()
	{
		setRasterizerState();
		setDepthStencilState();
		setBlendState();

		setVertexShader();
		//setHullShader();
		//setDomainShader();
		//setGeometryShader();
		setPixelShader();
	}
	void init(Shader *vertexShader, Shader *hullShader, Shader *domainShader, Shader *geometryShader, Shader *pixelShader)
	{
		assert(vertexShader);
		//assert(hullShader);
		//assert(domainShader);
		//assert(geometryShader);
		assert(pixelShader);

		_vertexShader = vertexShader;
		_hullShader = hullShader;
		_domainShader = domainShader;
		_geometryShader = geometryShader;
		_pixelShader = pixelShader;
	}
protected:
	void setRasterizerState()
	{
		//NoCull
	}
	void setDepthStencilState()
	{
		//DepthNormal 0
	}
	void setBlendState()
	{
		//NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF
	}
	void setVertexShader()
	{
		_vertexShader->bind();
	}
	void setHullShader()
	{
		_hullShader->bind();
	}
	void setDomainShader()
	{
		_domainShader->bind();
	}
	void setGeometryShader()
	{
		_geometryShader->bind();
	}
	void setPixelShader()
	{
		_pixelShader->bind();
	}
private:
	Shader *_vertexShader;
	Shader *_hullShader;
	Shader *_domainShader;
	Shader *_geometryShader;
	Shader *_pixelShader;
};
