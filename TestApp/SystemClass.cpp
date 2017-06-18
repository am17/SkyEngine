#include "SystemClass.h"
#include <iostream>
#include "Terrain.h"
#include "Skybox.h"
#include "Ocean.h"

using namespace std;

//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG

SystemClass::SystemClass()
	: m_applicationName(nullptr), 
	m_hinstance(nullptr), 
	m_hwnd(nullptr), 
	m_scene(nullptr),
	m_device(nullptr),
	m_efactory(nullptr)
	//m_render(nullptr)
{

}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
	if (inputHandler)
	{
		delete inputHandler;
		inputHandler = nullptr;
	}
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	inputHandler = new DirectInputHandler(m_hinstance, m_hwnd);

	m_device = new Device();
	m_device->init(m_hwnd, screenWidth, screenHeight, false);

	//m_render = new Render();

	//m_render->initDevice(m_hwnd, screenWidth, screenHeight, false);

	m_scene = new Scene();
	m_scene->init(m_device);

	m_scene->getCamera()->setPosition(786, 167, -301);
	//m_scene->init(m_render);

	m_efactory = new BaseEntityFactory(m_device);

	/*Entity* entity = m_efactory->createTeapotEntity();
	m_scene->add(entity);*/

	//Entity* terrainEntity = m_efactory->createEntity();

	Camera *cam = m_scene->getCamera();

	//Terrain* terrain = new Terrain(m_device, terrainEntity, cam);
	//terrain->init();

	//m_scene->add(terrain);

	//Entity* sphere = m_efactory->createGeoSphereEntity();

	//Skybox *sky = new Skybox(m_device, sphere, cam);

	//m_scene->add(sky);

	//Entity* oceanEntity = m_efactory->createEntity();

	//Ocean *ocean = new Ocean(m_device, oceanEntity, cam);
	//ocean->init();

	//m_scene->add(ocean);

	//Entity* debugQuad = m_efactory->createQuadEntity();
	//m_scene->add(debugQuad);

	//cam->setPosition(-441.0f, -10.0f, -489.0f);
	//cam->setPosition(-540.0f, -94.0f, -498.0f);
	//cam->setRotation(-1.6024, -3.14, 0);

	ICommand *wCommand = new SimpleCommand<Camera>(cam, &Camera::moveForward);
	ICommand *sCommand = new SimpleCommand<Camera>(cam, &Camera::moveBackward);
	ICommand *rmCommand = new SimpleCommand<Camera>(cam, &Camera::rotate);

	//ICommand *leftCommand = new SimpleCommand<ICamera>(cam, &ICamera::stepRotateLeft);
	//ICommand *rightCommand = new SimpleCommand<ICamera>(cam, &ICamera::stepRotateRight);
	//ICommand *upCommand = new SimpleCommand<ICamera>(cam, &ICamera::stepUp);
	//ICommand *downCommand = new SimpleCommand<ICamera>(cam, &ICamera::stepDown);
	//ICommand *rotateUpCommand = new SimpleCommand<ICamera>(cam, &ICamera::stepRotateUp);
	//ICommand *rotateDownCommand = new SimpleCommand<ICamera>(cam, &ICamera::stepRotateDown);
	//
	inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_W, wCommand);
	inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_S, sCommand);
	//inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_A, leftCommand);
	//inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_D, rightCommand);
	//inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_ARROWUP, wCommand);
	inputHandler->bindMouseCommand(EMOUSE_COMMAND::EMC_RIGHTBNT, rmCommand);
	//inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_ARROWLEFT, upCommand);
	//inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_ARROWRIGHT, downCommand);

	Texture* depthMap = m_device->getDepthMap();
	ICommand *saveScreenCommand = new SimpleCommand<Texture>(depthMap, &Texture::saveToFile);
	inputHandler->bindKeyCommand(EKEYBOARD_COMMAND::EKC_ARROWUP, saveScreenCommand);

	//Entity *cube = m_scene->addEntity(EBASIC_ENTITY::ENTITY_BOX);
	//cube->setScale(4.0f, 2.0f, 4.0f);
	//cube->getMesh()->getMaterial(0)->setVertexShader(EVertexShader::VS_SPECULAR_MAPPING);
	//cube->getMesh()->getMaterial(0)->setPixelShader(EPixelShader::PS_SPECULAR_MAPPING);
	//cube->getMesh()->getMaterial(0)->setTexture(L"Textures/stone02.dds", 0, ETEXTURE_MAP_TYPE::TYPE_DEFFUSE_MAP);
	//cube->getMesh()->getMaterial(0)->setTexture(L"Textures/bump02.dds", 1, ETEXTURE_MAP_TYPE::TYPE_NORMAL_MAP);
	//cube->getMesh()->getMaterial(0)->setTexture(L"Textures/spec02.dds", 2, ETEXTURE_MAP_TYPE::TYPE_SPECULAR_MAP);

	//Behavior *beh = new Behavior();
	//Entity *teapot = m_scene->addEntity(EBASIC_ENTITY::ENTITY_TEAPOT);
	//teapot->setPosition(0, 1.3, 0);
	//teapot->getMesh()->getMaterial(0)->setTexture(L"Textures/Texture2.dds", 0);
	//teapot->setBehavior(beh);

	//TerrainEntity *terrain = m_scene->addTerrainEntity("Data/heightmap.r16", 1025, 1025);
	//TerrainEntity *terrain = m_scene->addTerrainEntity("Textures/heightmap.bmp", 257, 257);
	//terrain->getMesh()->getMaterial(0)->setTexture(L"Textures/grass.dds", 0);
	//terrain->getMesh()->getMaterial(0)->setTexture(L"Textures/bump01.dds", 1);
	//terrain->setPosition(-512, -100, -512);
	//terrain->setScale(0.001,0.001,0.001);

	//m_scene->addSky();

	return true;
}

void SystemClass::Shutdown()
{
	/*if (m_render)
	{
		delete m_render;
		m_render = nullptr;
	}*/

	if (m_efactory)
	{
		delete m_efactory;
		m_efactory = nullptr;
	}

	if (m_device)
	{
		delete m_device;
		m_device = nullptr;
	}

	if (m_scene)
	{
		delete m_scene;
		m_scene = nullptr;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	DWORD inx = 0;

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			inputHandler->handleInput();
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

bool SystemClass::Frame() const
{
	Camera *cam = m_scene->getCamera();
	DIMOUSESTATE& mstate = inputHandler->getMouseState();
	cam->setMouseState(mstate);

	m_device->clearRenderTarget(ERenderTarget::RT_BACK_BUFFER);
	//if (m_render)
	//{
	//	//printf("FPS: %d\n", m_render->getFps());

		m_scene->update();

	//	m_scene->draw();
	//}
	m_device->present();

	//printf("cam: %f %f %f %\n", cam->getPosition().x, cam->getPosition().y, cam->getPosition().z);

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	//DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	screenWidth = 800;
	screenHeight = 600;

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

bool SystemClass::HandleInput(float frameTime)
{
	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
