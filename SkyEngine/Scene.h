#pragma once
#include "component.h"
#include "common.h"
#include "Entity.h"
#include <list>
#include "SceneUpdater.h"
#include "Camera.h"
#include "BackBufferRenderer.h"

using namespace std;

class Scene : public Composite
{
public:
	SKYENGINEDLL Scene();
	~Scene();
	SKYENGINEDLL void update();
	SKYENGINEDLL void add(Component *component) override;
	SKYENGINEDLL Camera* getCamera() const;
	SKYENGINEDLL void init(Device* pDevice);
private:
	list<Component*> _entities;
	SceneUpdater *pEntitiesUpdater;
	Camera* _pCamera;
	BackBufferRenderer *bbRenderer;
};

