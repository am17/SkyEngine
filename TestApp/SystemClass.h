#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Scene.h"
#include "Device.h"
#include "BaseEntityFactory.h"
//#include "Render.h"
#include "DirectInputHadler.h"

using namespace sky;
//using namespace scene;

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();
	bool HandleInput(float);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame() const;
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Scene *m_scene;
	Device *m_device;
	BaseEntityFactory *m_efactory;
	//Render *m_render;

	DirectInputHandler *inputHandler;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

