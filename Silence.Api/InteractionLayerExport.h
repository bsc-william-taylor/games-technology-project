
#pragma once

#if defined DLL_EXPORT 
    #define SILENCE_EXPORT_API __declspec(dllexport)
#else
    #define SILENCE_EXPORT_API __declspec(dllimport)
#endif

#pragma warning(disable: 4290)
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)

#pragma comment(lib, "Silence.Core.lib")