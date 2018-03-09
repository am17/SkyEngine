#ifdef SKYENGINEDLL_EXPORTS
#define SKYENGINEDLL __declspec(dllexport) 
#else
#define SKYENGINEDLL __declspec(dllimport) 
#endif