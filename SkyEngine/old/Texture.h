#pragma once
#include "ITexture.h"

namespace sky
{
	class Texture2D : public ITexture
	{
	public:
		Texture2D();
		Texture2D(WCHAR* filename, ETEXTURE_MAP_TYPE type = ETEXTURE_MAP_TYPE::TYPE_DEFFUSE_MAP);
		~Texture2D();
		void createFromFile(WCHAR* filename, ETEXTURE_MAP_TYPE type = ETEXTURE_MAP_TYPE::TYPE_DEFFUSE_MAP);
		IShaderResource* getResource() const;
		ETEXTURE_MAP_TYPE getType() const;
		IShaderResource *getShaderResource() const override;
	protected:
		IShaderResource *m_shader_resource;
		ETEXTURE_MAP_TYPE m_mapType;
	};
}

