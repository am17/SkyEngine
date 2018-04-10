#ifdef SKYENGINEDLL_EXPORTS
#define SKYENGINEDLL __declspec(dllexport) 
#else
#define SKYENGINEDLL __declspec(dllimport) 
#endif

#pragma warning(push)
#pragma warning(disable : 4005)	// macro redefinition