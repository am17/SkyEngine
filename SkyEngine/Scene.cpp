#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
	:_pCamera(nullptr),
	bbRenderer(nullptr)
{
	
}


Scene::~Scene()
{
	if (bbRenderer)
	{
		delete bbRenderer;
		bbRenderer = nullptr;
	}

	if (_pCamera)
	{
		delete _pCamera;
		_pCamera = nullptr;
	}

	if (pEntitiesUpdater)
	{
		delete pEntitiesUpdater;
		pEntitiesUpdater = nullptr;
	}
}

void Scene::add(Component *component)
{
	_pCamera->registerObserver(component);

	_entities.push_back(component);
}

void Scene::update()
{
	_pCamera->update();

	pEntitiesUpdater->update();

	bbRenderer->render(nullptr);
}


Camera* Scene::getCamera() const
{
	return _pCamera;
}

void Scene::init(Device * pDevice)
{
	_pCamera = new Camera(800, 600, 1.0f, 20000.f);
	pEntitiesUpdater = new SceneUpdater(_entities);

	bbRenderer = new BackBufferRenderer(pDevice);
}
