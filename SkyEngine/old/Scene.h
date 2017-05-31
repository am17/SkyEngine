#pragma once
#include "IDevice.h"
#include "ICamera.h"
#include "VertexFormat.h"
#include "StaticMesh.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Render.h"
#include <set>
#include "RenderTexture.h"

using namespace sky;
using namespace render;
using namespace std;

namespace scene
{
	class Scene
	{
	public:
		SKYENGINEDLL Scene();
		SKYENGINEDLL ~Scene();
		SKYENGINEDLL bool init(Render *render);
		SKYENGINEDLL void update();
		SKYENGINEDLL bool draw();
		SKYENGINEDLL Entity* addEntity(EBASIC_ENTITY entityType);
		SKYENGINEDLL TerrainEntity* addTerrainEntity(const char* heightMapFile, int terrainHeight, int terrainWidth);
		SKYENGINEDLL Entity* addSky();
		SKYENGINEDLL ICamera* getCamera() const;
		void renderToTexture(IRenderTargetResources *renderTargetResources);
	private:
		void drawSkyDome();
		void drawTerrain();
		void drawStatic();
		void updateSkyDome() const;
		void updateTerrain();
		void updateStatic();
		Render *m_render;
		ICamera *m_camera;
		EntityFactory *m_entityFactory;
		Entity* m_skyDomeEntity;
		vector<Entity*> sceneStaticEntities;
		vector<TerrainEntity*> sceneTerrainEntities;
		CBPerFrame *cbPerFrame;
		SCameraBuffer *cbCamera;

		//RenderTexture *testText;
		ColorMesh *frustumDebugMesh;
		CBPerObject *cbPerObject;
	};
}

