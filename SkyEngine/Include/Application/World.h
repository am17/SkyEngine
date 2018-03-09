#pragma once
#include <memory>
#include "Systems\RenderSystem.h"
#include "common.h"

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
	std::unique_ptr<RenderSystem> renderSystem;
};