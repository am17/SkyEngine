#include "systemclass.h"

//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main()
{
	//ObjToTxtConverter *loader = new ObjToTxtConverter();

	//loader->ConvertToTxt("cube.obj", "model2.txt");

	bool result;

	// Create the system object.
	SystemClass* System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}