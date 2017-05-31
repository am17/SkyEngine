#pragma once
#include <d3d11.h>

namespace sky
{
	enum class ETEXTURE_MAP_TYPE
	{
		TYPE_DEFFUSE_MAP	= 0,
		TYPE_NORMAL_MAP		= 1,
		TYPE_SPECULAR_MAP	= 2,
		TYPE_REFLECTION_MAP = 3,
		TYPE_OPACITY_MAP	= 4
	};

	class IShaderResource;

	class ITexture
	{
	public:
		virtual ~ITexture() {}
		virtual IShaderResource *getShaderResource() const = 0;
	};

	class IShaderResource
	{
	public:
		virtual ~IShaderResource() {}
	};

	class D3D11ShaderResource : public IShaderResource
	{
	public:
		D3D11ShaderResource(ID3D11ShaderResourceView* resource_view) :
			m_texture_resource_view(resource_view)
		{}
		~D3D11ShaderResource()
		{
			if (m_texture_resource_view)
			{
				m_texture_resource_view->Release();
				m_texture_resource_view = nullptr;
			}
		}
		ID3D11ShaderResourceView* getShaderResource() const
		{
			return m_texture_resource_view;
		}
	private:
		ID3D11ShaderResourceView* m_texture_resource_view;
	};

	class IRenderTargetResources
	{
	public:
		virtual ~IRenderTargetResources() {}
		virtual IShaderResource *getShaderResource() = 0;
	};

	class D3D11RenderTargetResources : public IRenderTargetResources
	{
	public:
		D3D11RenderTargetResources(ID3D11Texture2D* renderTargetTexture, ID3D11RenderTargetView* renderTargetView, D3D11ShaderResource* shaderResource)
			:m_renderTargetTexture(renderTargetTexture),
			m_renderTargetView(renderTargetView),
			m_shaderResource(shaderResource)
		{}
		~D3D11RenderTargetResources() 
		{
			if (m_shaderResource)
			{
				delete m_shaderResource;
				m_shaderResource = nullptr;
			}

			if (m_renderTargetView)
			{
				m_renderTargetView->Release();
				m_renderTargetView = nullptr;
			}

			if (m_shaderResource)
			{
				m_renderTargetTexture->Release();
				m_renderTargetTexture = nullptr;
			}

		}
		ID3D11RenderTargetView* getRenderTarget() const
		{
			return m_renderTargetView;
		}
		ID3D11Texture2D* getTargetTexture() const
		{
			return m_renderTargetTexture;
		}
		IShaderResource *getShaderResource() override
		{
			return m_shaderResource;
		}
	private:
		ID3D11Texture2D* m_renderTargetTexture;
		ID3D11RenderTargetView* m_renderTargetView;
		D3D11ShaderResource* m_shaderResource;
	};
}
