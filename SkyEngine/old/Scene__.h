#pragma once
#include "IDevice.h"
#include "ICamera.h"
#include "VertexFormat.h"
#include "RenderTemplate.h"
#include "IEntity.h"
#include "StaticMesh.h"
#include "Entity.h"
#include "Render.h"
#include "EntityFactory.h"

using namespace sky;
using namespace render;

namespace scene
{
	class Scene
	{
	public:
		SKYENGINEDLL Scene();
		SKYENGINEDLL ~Scene();
		SKYENGINEDLL bool init(Render* render);
		SKYENGINEDLL bool draw() const;
		SKYENGINEDLL bool addSceneNode();
	private:
		Render *m_render;
		ICamera *m_camera;
		EntityFactory *entityFactory;

		Entity *testEntity;
		StaticMesh *mesh;
	};
}

