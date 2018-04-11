#include "Application\Application.h"
#include "Components\RenderComponent.h"
#include "Systems\RenderSystem\Mesh.h"

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

	if (!world->Init(window->GetHWND(), screenWidth, screenHeight))
	{
		return false;
	}

	//
	Entity *testEntity = new Entity();

	Mesh *mesh = world->GetRenderSystem()->GetMeshFactory()->CreateTriangle();

	Shader *vertexShader = world->GetRenderSystem()->GetShaderFactory()->GetShader(L"colorVS", SHADER_TYPE::VERTEX_SHADER, "main");
	Shader *pixelShader = world->GetRenderSystem()->GetShaderFactory()->GetShader(L"colorPS", SHADER_TYPE::PIXEL_SHADER, "main");

	RenderComponent *renderComponent = new RenderComponent(mesh);

	testEntity->Add(renderComponent);

	world->AddEntity(testEntity);
	//

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