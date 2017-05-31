#include "stdafx.h"
#include "Material.h"
#include "Texture.h"

namespace sky
{
	Material::Material()
		:m_texturesCount(0)
	{

	}

	Material::~Material()
	{
		for (vector<MaterialObserver*>::const_iterator it = m_materialObservers.begin(); it != m_materialObservers.end(); ++it)
		{
			MaterialObserver *observer = (*it);
			if (observer)
			{
				delete observer;
				observer = nullptr;
			}
		}

		for (size_t i = 0; i < textures.size(); i++)
		{
			if (textures[i])
			{
				delete textures[i];
				textures[i] = nullptr;
			}
		}
	}

	void Material::setTexture(wchar_t* fileName, unsigned int textureIndex, ETEXTURE_MAP_TYPE type)
	{
		assert(textureIndex < m_texturesCount);

		if (textureIndex >= 0 && fileName)
		{
			Texture2D *texture = new Texture2D(fileName, type);

			if (textures.size() <= textureIndex)
			{
				textures.push_back(texture);
			}
			else
			{
				if (textures[textureIndex])
				{
					delete textures[textureIndex];
					textures[textureIndex] = nullptr;
				}

				textures[textureIndex] = texture;
			}

			for (vector<MaterialObserver*>::const_iterator it = m_materialObservers.begin(); it != m_materialObservers.end(); ++it)
			{
				MaterialObserver *observer = (*it);
				if (observer)
				{
					IShaderResource *shader_resource = texture->getResource();
					observer->onTextureUpdated(shader_resource, textureIndex);
				}
			}
		}
	}

	void Material::setTexture(ITexture* texture, unsigned textureIndex)
	{
		assert(textureIndex < m_texturesCount);

		if (textureIndex >= 0 && texture)
		{
			if (textures.size() <= textureIndex)
			{
				textures.push_back(texture);
			}
			else
			{
				if (textures[textureIndex])
				{
					delete textures[textureIndex];
					textures[textureIndex] = nullptr;
				}

				textures[textureIndex] = texture;
			}

			for (vector<MaterialObserver*>::const_iterator it = m_materialObservers.begin(); it != m_materialObservers.end(); ++it)
			{
				MaterialObserver *observer = (*it);
				if (observer)
				{
					IShaderResource *shader_resource = texture->getShaderResource();
					observer->onTextureUpdated(shader_resource, textureIndex);
				}
			}
		}
	}

	void Material::updateTexture(ITexture* texture, unsigned textureIndex)
	{
		assert(textureIndex < m_texturesCount);

		for (vector<MaterialObserver*>::const_iterator it = m_materialObservers.begin(); it != m_materialObservers.end(); ++it)
		{
			MaterialObserver *observer = (*it);
			if (observer)
			{
				IShaderResource *shader_resource = texture->getShaderResource();
				observer->onTextureUpdated(shader_resource, textureIndex);
			}
		}
	}

	ITexture *Material::getTexture(unsigned textureIndex) const
	{
		assert(textureIndex < textures.size());

		return textures[textureIndex];
	}

	size_t Material::getTextureCount() const
	{
		return textures.size();
	}

	void Material::setTextureCount(size_t textureCount)
	{
		m_texturesCount = textureCount;

		textures.reserve(m_texturesCount);
		textures.resize(m_texturesCount);
	}

	void Material::addObserver(MaterialObserver* observer)
	{
		m_materialObservers.push_back(observer);
	}

	void Material::setVertexShader(EVertexShader shader)
	{
		for (vector<MaterialObserver*>::const_iterator it = m_materialObservers.begin(); it != m_materialObservers.end(); ++it)
		{
			MaterialObserver *observer = (*it);
			if (observer)
			{
				observer->onVertexShaderChanged(shader, this);
			}
		}
	}

	void Material::setPixelShader(EPixelShader shader)
	{
		for (vector<MaterialObserver*>::const_iterator it = m_materialObservers.begin(); it != m_materialObservers.end(); ++it)
		{
			MaterialObserver *observer = (*it);
			if (observer)
			{
				observer->onPixelShaderChanged(shader, this);
			}
		}
	}

	SMaterial Material::getAttributes() const
	{
		return m_attributes;
	}
}
