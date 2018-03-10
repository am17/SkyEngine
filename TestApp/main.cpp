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

	bool result = app->Init();

	if (result)
	{
		app->Run();

		app->Shutdown();
	}

	return 0;
}