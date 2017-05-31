#include "stdafx.h"
#include "Texture.h"

#include "globals.h"

namespace sky
{
	Texture2D::Texture2D(): 
		m_shader_resource(nullptr), 
		m_mapType(ETEXTURE_MAP_TYPE::TYPE_DEFFUSE_MAP)
	{
	}

	Texture2D::Texture2D(WCHAR* filename, ETEXTURE_MAP_TYPE type) 
		: m_shader_resource(nullptr),
		m_mapType(type)
	{
		createFromFile(filename, type);
	}

	Texture2D::~Texture2D()
	{
		if (m_shader_resource)
		{
			delete m_shader_resource;
			m_shader_resource = nullptr;
		}
	}

	void Texture2D::createFromFile(WCHAR* filename, ETEXTURE_MAP_TYPE type)
	{
		IDevice *device = global::getDevice();

		assert(device != nullptr);

		m_shader_resource = device->createShaderResource(filename);

		assert(m_shader_resource != nullptr);

		m_mapType = type;
	}

	IShaderResource* Texture2D::getResource() const
	{
		return m_shader_resource;
	}

	ETEXTURE_MAP_TYPE Texture2D::getType() const
	{
		return m_mapType;
	}

	IShaderResource* Texture2D::getShaderResource() const
	{
		return m_shader_resource;
	}
}
