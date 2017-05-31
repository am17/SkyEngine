#include "stdafx.h"
#include "Scene.h"
#include "Camera.h"
#include "Entity.h"
#include "Log.h"

namespace scene
{
	Scene::Scene()
		: m_render(nullptr),
		m_camera(nullptr),
		m_entityFactory(nullptr),
		m_skyDomeEntity(nullptr),
		cbPerFrame(nullptr), 
		cbCamera(nullptr), 
		frustumDebugMesh(nullptr)
	//testText(nullptr)
	{
	}

	Scene::~Scene()
	{
		if (cbCamera)
		{
			delete cbCamera;
			cbCamera = nullptr;
		}

		if (cbPerFrame)
		{
			delete cbPerFrame;
			cbPerFrame = nullptr;
		}

		for (vector<Entity*>::const_iterator it = sceneStaticEntities.begin(); it != sceneStaticEntities.end(); ++it) 
		{
			Entity *entity = (*it);
			if (entity)
			{
				delete entity;
				entity = nullptr;
			}
		}
		sceneStaticEntities.clear();

		if (m_entityFactory)
		{
			delete m_entityFactory;
			m_entityFactory = nullptr;
		}
		
		if (m_camera)
		{
			delete m_camera;
			m_camera = nullptr;
		}
	}

	bool Scene::init(Render *render)
	{
		m_render = render;

		m_entityFactory = new EntityFactory(m_render->getDevice(), m_render->getShaderManager());

		m_camera = new Camera();
		if (!m_camera)
		{
			return false;
		}

		m_camera->setPosition(0.0f, 2.0f, -10.0f);

		cbPerFrame = new CBPerFrame();

		cbPerFrame->directionalLight.ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
		cbPerFrame->directionalLight.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//cbPerFrame->directionalLight.lightDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
		cbPerFrame->directionalLight.lightDirection = XMFLOAT3(0.0f, -1.0f, 0.0f);
		cbPerFrame->directionalLight.specularPower = 32.0f;
		cbPerFrame->directionalLight.specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		/*cbPerFrame->pointLight.ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
		cbPerFrame->pointLight.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		cbPerFrame->pointLight.position = XMFLOAT3(0.0f, 0.0f, 1.0f);
		cbPerFrame->pointLight.specularPower = 32.0f;
		cbPerFrame->pointLight.specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		cbPerFrame->pointLight.range = 100.0f;
		cbPerFrame->pointLight.attenuation = XMFLOAT3(0.0f, 0.2f, 0.0f);*/

		//Расчет вектора полпути (для расчета отраженного света по модели Блинна-Фонга)
		XMVECTOR lightDirection = XMLoadFloat3(&cbPerFrame->directionalLight.lightDirection);
		//XMVECTOR lightDirection = XMLoadFloat3(&cbPerFrame->pointLight.position);
		XMFLOAT3 camPos = m_camera->getPosition();
		XMVECTOR viewDirection = XMLoadFloat3(&camPos);
		XMVECTOR directionToLight = -lightDirection;
		XMVECTOR halfWayVector = XMVector3Normalize(directionToLight + viewDirection);
		XMStoreFloat3(&cbPerFrame->directionalLight.halfWayVector, halfWayVector);
		//XMStoreFloat3(&cbPerFrame->pointLight.halfWayVector, halfWayVector);

		cbCamera = new SCameraBuffer();
		cbCamera->cameraPosition = m_camera->getPosition();
		cbCamera->padding = 0.0f;
		cbCamera->reflectionMatrix = m_camera->getViewMatrix();
		cbCamera->projection = m_render->getProjectMatrix();

		m_camera->setProjectionMatrix(m_render->getProjectMatrix());

		//testText = new RenderTexture(800, 600);

		vector<ColorVertex> vertices(8);
		vector<unsigned int> indices(24);

		XMFLOAT3 corners[8];

		m_camera->getFrustum()->GetCorners(corners);

		for (size_t i = 0; i < 8; i++)
		{
			vertices[i] = ColorVertex(corners[i], XMFLOAT4(1, 0, 0, 1));
		}

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 1;
		indices[3] = 2;
		indices[4] = 2;
		indices[5] = 3;
		indices[6] = 3;
		indices[7] = 0;

		indices[8] = 4;
		indices[9] = 5;
		indices[10] = 5;
		indices[11] = 6;
		indices[12] = 6;
		indices[13] = 7;
		indices[14] = 7;
		indices[15] = 4;

		indices[16] = 0;
		indices[17] = 4;
		indices[18] = 1;
		indices[19] = 5;
		indices[20] = 2;
		indices[21] = 6;
		indices[22] = 3;
		indices[23] = 7;

		frustumDebugMesh = m_entityFactory->getGeometryCreator()->createColorMesh(vertices, indices);

		cbPerObject = new CBPerObject();

		return true;
	}

	void Scene::update()
	{
		static float textureTranslation = 0.0f;
		textureTranslation += 0.00005f;
		if (textureTranslation > 1.0f)
		{
			textureTranslation -= 1.0f;
		}

		cbPerFrame->directionalLight.textureTranslation = textureTranslation;

		//printf("%f\n", elapsed);

		/*Timer *timer = m_render->getTimer();
		double currentTime = timer->getCurrentTime();
		double lastTime = timer->getLastTime();
		double elapsed = currentTime - lastTime;*/

		m_camera->update();

		/*XMFLOAT3 CP = m_camera->getPosition();

		printf("%f %f %f\n", CP.x,CP.y, CP.z);*/

		//
		XMFLOAT4X4 frustumWVP;

		XMFLOAT4X4 viewMat = m_camera->getViewMatrix();
		XMFLOAT4X4 projMat = m_render->getProjectMatrix();
		XMMATRIX viewMatrix = XMLoadFloat4x4(&viewMat);
		XMMATRIX projectMatrix = XMLoadFloat4x4(&projMat);

		XMMATRIX WVP = XMMatrixIdentity() * viewMatrix * projectMatrix;

		XMStoreFloat4x4(&frustumWVP, XMMatrixTranspose(WVP));

		cbPerObject->WVP = frustumWVP;

		frustumDebugMesh->getRenderData()->updateConstantBuffer<EShaderType::ST_Vertex>(0, cbPerObject);
		//

		updateSkyDome();

		updateTerrain();
		
		updateStatic();
	}

	bool Scene::draw()
	{
		m_render->beginScene();

		drawSkyDome();

		drawTerrain();

		drawStatic();

		m_render->endScene();

		return true;
	}

	Entity* Scene::addEntity(EBASIC_ENTITY entityType)
	{
		Entity* Entity = nullptr;

		switch (entityType)
		{
		case sky::EBASIC_ENTITY::ENTITY_CUBE:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_CUBE>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_TEAPOT:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_TEAPOT>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_SPHERE:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_SPHERE>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_BOX:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_BOX>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_GEOSPHERE:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_GEOSPHERE>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_CYLINDER:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_CYLINDER>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_CONE:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_CONE>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_TORUS:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_TORUS>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_TETRAHEDRON:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_TETRAHEDRON>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_OCTAHEDRON:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_OCTAHEDRON>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_DODECAHEDRON:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_DODECAHEDRON>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_ICOSAHEDRON:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_ICOSAHEDRON>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_PLANE:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_PLANE>();
			break;
		case sky::EBASIC_ENTITY::ENTITY_TRIANGLE:
			Entity = m_entityFactory->create<EBASIC_ENTITY::ENTITY_TRIANGLE>();
		default:
			break;
		}

		assert(Entity != nullptr);

		sceneStaticEntities.push_back(Entity);

		return Entity;
	}

	TerrainEntity* Scene::addTerrainEntity(const char* heightMapFile, int terrainHeight, int terrainWidth)
	{
		TerrainEntity* Entity = m_entityFactory->createTerrainEntity(heightMapFile, terrainHeight, terrainWidth);

		assert(Entity != nullptr);

		sceneTerrainEntities.push_back(Entity);

		return Entity;
	}

	Entity* Scene::addSky()
	{
		Entity* Entity = m_entityFactory->createSkydomeEntity();

		assert(Entity != nullptr);

		m_skyDomeEntity = Entity;

		return Entity;
	}

	ICamera* Scene::getCamera() const
	{
		assert(m_camera != nullptr);

		return m_camera;
	}

	void Scene::renderToTexture(IRenderTargetResources *renderTargetResources)
	{
		m_render->getDevice()->setRenderTarget(renderTargetResources);

		m_render->getDevice()->clearRenderTarget(renderTargetResources);

		m_camera->updateReflectViewMatrix(-1.5f);

		Entity *entity = sceneStaticEntities[0];
		if (entity)
		{
			entity->update(m_camera->getReflectionViewMatrix(), m_render->getProjectMatrix());

			StaticMeshRenderData *renderData = entity->getMesh()->getRenderData();

			m_render->bindRenderData(renderData);
		}

		entity = sceneStaticEntities[1];
		if (entity)
		{
			entity->update(m_camera->getReflectionViewMatrix(), m_render->getProjectMatrix());

			StaticMeshRenderData *renderData = entity->getMesh()->getRenderData();

			m_render->bindRenderData(renderData);
		}

		m_render->getDevice()->resetRenderTarget();
	}

	void Scene::drawSkyDome()
	{
		if (m_skyDomeEntity)
		{
			StaticMeshRenderData *renderData = m_skyDomeEntity->getMesh()->getRenderData();

			m_render->bindRenderData(renderData);
		}
	}

	void Scene::drawTerrain()
	{
		//m_render->bindRenderData(frustumDebugMesh->getRenderData());

		for (vector<TerrainEntity*>::const_iterator it = sceneTerrainEntities.begin(); it != sceneTerrainEntities.end(); ++it)
		{
			TerrainEntity *entity = (*it);
			if (entity)
			{
				TerrainMesh* mesh = entity->getMesh();
				size_t cellsCount = mesh->getCellsCount();

				m_render->bindRenderData(mesh->getRenderData());

				for (size_t i = 0; i < cellsCount; i++)
				{
					TerrainCell* cell = mesh->getCell(i);

					if (cell->isVisible())
					{
						//m_render->bindRenderData(cell->getDebugMesh()->getRenderData());
					}
				}
			}
		}
	}

	void Scene::drawStatic()
	{
		for (vector<Entity*>::const_iterator it = sceneStaticEntities.begin(); it != sceneStaticEntities.end(); ++it)
		{
			Entity *entity = (*it);
			if (entity)
			{
				StaticMeshRenderData *renderData = entity->getMesh()->getRenderData();

				m_render->bindRenderData(renderData);
			}
		}
	}

	void Scene::updateSkyDome() const
	{
		if (m_skyDomeEntity)
		{
			m_skyDomeEntity->update(m_camera->getViewMatrix(), m_render->getProjectMatrix());

			XMFLOAT3 capPos = m_camera->getPosition();

			m_skyDomeEntity->setPosition(capPos.x, capPos.y, capPos.z);

			CBPerObject *cbPerObject = m_skyDomeEntity->getTransform();
			m_skyDomeEntity->getMesh()->getRenderData()->updateConstantBuffer<EShaderType::ST_Vertex>(0, cbPerObject);
			m_skyDomeEntity->getMesh()->getRenderData()->updateConstantBuffer<EShaderType::ST_Vertex>(1, cbCamera);
		}
	}
	void Scene::updateTerrain()
	{
		XMFLOAT3 Corners[8];
		BoundingFrustum frustum = *m_camera->getFrustum();
		frustum.GetCorners(Corners);

		XMFLOAT3 camTarget = m_camera->getTarget();
		XMVECTOR V1 = XMLoadFloat3(&camTarget);

		for (vector<TerrainEntity*>::const_iterator it = sceneTerrainEntities.begin(); it != sceneTerrainEntities.end(); ++it)
		{
			TerrainEntity *entity = (*it);
			if (entity)
			{
				entity->update(m_camera->getViewMatrix(), m_render->getProjectMatrix());

				CBPerObject *cbPerObject = entity->getTransform();

				TerrainMesh* mesh = entity->getMesh();
				StaticMeshRenderData* renderData = mesh->getRenderData();

				renderData->updateConstantBuffer<EShaderType::ST_Vertex>(0, cbPerObject);
				renderData->updateConstantBuffer<EShaderType::ST_Vertex>(1, cbCamera);
				renderData->updateConstantBuffer<EShaderType::ST_Pixel>(0, cbPerFrame);

				size_t cellsCount = mesh->getCellsCount();

				int countRender = 0;
				for (size_t i = 0; i < cellsCount; i++)
				{
					TerrainCell* cell = mesh->getCell(i);

					/*CBPerObject *transform = entity->getTransform();
					XMFLOAT4X4 viewMat = m_camera->getViewMatrix();
					XMFLOAT4X4 projMat = m_render->getProjectMatrix();

					cell->updateCB(transform);*/

					BoundingBox aabb = *cell->getBoundingBox();

					//BoundingBox test;
					//aabb.Transform(test, XMMatrixTranslation(0, 0, 0));

					XMVECTOR V2 = XMVectorSet(aabb.Center.x, aabb.Center.y, aabb.Center.z, 0);

					XMVECTOR result =  XMVector3Cross(V1, V2);

					ContainmentType conType = frustum.Contains(aabb);

					cell->isVisible(conType != ContainmentType::DISJOINT);

					renderData->getPipelineState()->getDrawCall(i)->BeCalled = cell->isVisible();

					if (cell->isVisible())
					{
						countRender++;
					}
				}
				printf("%d\n", countRender);
			}
		}
	}
	void Scene::updateStatic()
	{
		for (vector<Entity*>::const_iterator it = sceneStaticEntities.begin(); it != sceneStaticEntities.end(); ++it)
		{
			Entity *entity = (*it);
			if (entity)
			{
				entity->update(m_camera->getViewMatrix(), m_render->getProjectMatrix());

				CBPerObject *cbPerObject = entity->getTransform();
				SMaterial material = entity->getMesh()->getMaterial(0)->getAttributes();

				entity->getMesh()->getRenderData()->updateConstantBuffer<EShaderType::ST_Vertex>(0, cbPerObject);
				entity->getMesh()->getRenderData()->updateConstantBuffer<EShaderType::ST_Vertex>(1, cbCamera);
				entity->getMesh()->getRenderData()->updateConstantBuffer<EShaderType::ST_Pixel>(0, cbPerFrame);
			}
		}
	}
}
