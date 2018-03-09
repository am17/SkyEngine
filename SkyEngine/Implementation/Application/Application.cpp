#include "Application\Application.h"

Application::Application()
{
	world = std::make_unique<World>();
	window = std::make_unique<WindowsWindow>();
}

Application::~Application()
{
}

bool Application::Init()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (!window->Create(screenWidth, screenHeight))
	{
		return false;
	}

	if (!world->Init())
	{
		return false;
	}

	world->GetRenderSystem()->Init(window->GetHWND(), screenWidth, screenHeight);

	return true;
}

void Application::Run()
{
	world->Start();

	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			world->Update();
		}
	}

	return;
}

void Application::Shutdown()
{
	world->Stop();
}