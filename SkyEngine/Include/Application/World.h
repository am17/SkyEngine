#pragma once
#include <memory>
#include "Systems\RenderSystem.h"
#include "common.h"
#include "StepTimer.h"
#include "Entity\Entity.h"
#include <vector>

class SKYENGINEDLL World
{
public:
	World();
	~World();
	void Update();
	void Start();
	void Stop();
	bool Init(HWND hWnd, int width, int height);
	void AddEntity(Entity *entity);
private:
	DX::StepTimer m_timer;
	std::unique_ptr<RenderSystem> renderSystem;
	std::vector<Entity*> entities;
};