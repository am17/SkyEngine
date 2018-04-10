#pragma once
#include <memory>
#include "Systems\RenderSystem.h"
#include "common.h"
#include "StepTimer.h"

class SKYENGINEDLL World
{
public:
	World();
	~World();
	void Update();
	void Start();
	void Stop();
	bool Init();
	RenderSystem *GetRenderSystem() const;
private:
	DX::StepTimer m_timer;
	std::unique_ptr<RenderSystem> renderSystem;
};