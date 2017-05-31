#pragma once
#include "DirectXMath.h"
#include  <vector>
#include "Texture.h"
#include "PSStage.h"
#include "VSStage.h"
#include "ShaderTypes.h"
#include "globals.h"
#include "RenderTexture.h"

using namespace DirectX;
using namespace std;
using namespace sky;

namespace sky
{
	class MaterialObserver;

	struct SMaterial
	{
		SMaterial()
			:Ambient(XMFLOAT4(1.0, 0.0, 0.0, 0))
			/*Diffuse(XMFLOAT4(0.0, 1.0, 0.0, 0)),
			Specular(XMFLOAT4(0.0, 0.0, 0.0, 0)),
			Emission(XMFLOAT4(0.0, 0.0, 0.0, 0)),
			Refraction(XMFLOAT3(0.0, 0.0, 0.0)),
			Shininess(0)*/
		{}

		XMFLOAT4 Ambient;
		/*XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		XMFLOAT4 Emission;
		XMFLOAT3 Refraction;
		float Shininess;*/
	};

	class IMaterial
	{
	public:
		virtual ~IMaterial() {}
		virtual void setTexture(wchar_t *fileName, unsigned int textureIndex, ETEXTURE_MAP_TYPE type = ETEXTURE_MAP_TYPE::TYPE_DEFFUSE_MAP) = 0;
		virtual void setTexture(ITexture *texture, unsigned int textureIndex) = 0;
		virtual void updateTexture(ITexture *texture, unsigned int textureIndex) = 0;
		virtual ITexture *getTexture(unsigned int textureIndex) const = 0;
		virtual size_t getTextureCount() const = 0;
		virtual void setTextureCount(size_t textureCount) = 0;
		virtual void addObserver(MaterialObserver *observer) = 0;
		virtual void setVertexShader(EVertexShader shader) = 0;
		virtual void setPixelShader(EPixelShader shader) = 0;
		virtual SMaterial getAttributes() const = 0;
	};

	class MaterialObserver
	{
	public:
		MaterialObserver(PSStage &psStage, VSStage &vsStage) 
			:m_psStage(&psStage),
			m_vsStage(&vsStage)
		{

		}
		~MaterialObserver(){}
		void onTextureUpdated(IShaderResource *shader_resource, unsigned int textureIndex) const
		{
			m_psStage->setShaderResource(shader_resource, textureIndex);
		}
		void onVertexShaderChanged(EVertexShader shader, IMaterial *material) const
		{
			ShaderManager *shaderManager = global::getShaderManager();

			assert(shaderManager != nullptr);

			IVertexShader *vShader = shaderManager->getVertexShader(shader);

			assert(vShader != nullptr);

			m_vsStage->setVertexShader(vShader);
		}
		void onPixelShaderChanged(EPixelShader shader, IMaterial *material) const
		{
			ShaderManager *shaderManager = global::getShaderManager();

			assert(shaderManager != nullptr);

			IPixelShader *pShader = shaderManager->getPixelShader(shader);

			assert(pShader != nullptr);

			m_psStage->setPixelShader(pShader);

			material->setTextureCount(pShader->getShaderResourceCount());
		}
	private:
		PSStage *m_psStage;
		VSStage *m_vsStage;
	};
}
