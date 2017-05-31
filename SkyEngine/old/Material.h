#pragma once
#include "IMaterial.h"

namespace sky
{
	class Material : public IMaterial
	{
	public:
		Material();
		~Material();
		void setTexture(wchar_t* fileName, unsigned int textureIndex, ETEXTURE_MAP_TYPE type) override;
		void setTexture(ITexture *texture, unsigned int textureIndex) override;
		void updateTexture(ITexture *texture, unsigned int textureIndex) override;
		ITexture *getTexture(unsigned int textureIndex) const override;
		size_t getTextureCount() const override;
		void setTextureCount(size_t textureCount) override;
		void addObserver(MaterialObserver *observer) override;
		void setVertexShader(EVertexShader shader) override;
		void setPixelShader(EPixelShader shader) override;
		SMaterial getAttributes() const;
	private:
		////рассеянный свет, исходящий от материала
		//XMFLOAT4 diffuseColor;
		////определяет окружающий свет
		//XMFLOAT4 ambientColor;
		////определяет отражающий (зеркальный) свет
		//XMFLOAT4 specularColor;
		////определяет излучающий свет материала
		//XMFLOAT4 emissiveColor;
		//float metallic;
		//float specular;
		////шероховатость
		//float roughness;
		//float opacity;
		////преломление: Air:1.0 Water : 1.333 Ice : 1.3 Glass : ~1.6 Diamond : 2.42
		//float refraction;
		//XMFLOAT3 normal;
		SMaterial m_attributes;
		vector<ITexture*> textures;
		size_t m_texturesCount;
		vector<MaterialObserver *> m_materialObservers;
	};
}
