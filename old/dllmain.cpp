// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(_WIN32_WCE)
#include <crtdbg.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(__BORLANDC__) && !defined (_WIN32_WCE)
				_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
		#endif
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

