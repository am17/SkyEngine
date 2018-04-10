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

bool World::Init(HWND hWnd, int width, int height)
{
	renderSystem = std::make_unique<RenderSystem>();

	renderSystem->Init(hWnd, width, height);

	return true;
}

void World::AddEntity(Entity * entity)
{
	renderSystem->RegisterEntity(entity);

	entities.push_back(entity);
}

void World::Update()
{
	m_timer.Tick([&]() {

		double time = m_timer.GetTotalSeconds();
		renderSystem->Update(time);

	});
}