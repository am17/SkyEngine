#include "systemclass.h"
#include "Include\Application\Application.h"
#include <memory>

//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main()
{
	std::unique_ptr<Application> app = std::make_unique<Application>();

	if (!app)
	{
		return 0;
	}

	bool result = app->Init();

	if (result)
	{
		app->Run();

		app->Shutdown();
	}

	/*SystemClass* System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = 0;*/

	return 0;
}