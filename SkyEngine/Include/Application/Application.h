#pragma once
#include <memory>
#include "Application\World.h"
#include "Application\WindowsWindow.h"

class SKYENGINEDLL Application
{
public:
	Application();
	~Application();
	bool Init();
	void Run();
	void Shutdown();
private:
	std::unique_ptr<World> world;
	std::unique_ptr<WindowsWindow> window;
};
