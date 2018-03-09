#include "Application\World.h"

World::World()
{
	
}

World::~World()
{
	
}

void World::Start()
{
	renderSystem->Start();

	return;
}

void World::Stop()
{
	renderSystem->Stop();
}

bool World::Init()
{
	renderSystem = std::make_unique<RenderSystem>();

	return true;
}

RenderSystem * World::GetRenderSystem() const
{
	return renderSystem.get();
}

void World::Update()
{

}