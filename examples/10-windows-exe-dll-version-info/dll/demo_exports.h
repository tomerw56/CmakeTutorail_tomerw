#pragma once

#ifdef _WIN32
    #ifdef BUILDING_DEMO_DLL
        #define DEMO_API __declspec(dllexport)
    #else
        #define DEMO_API __declspec(dllimport)
    #endif
#else
    #define DEMO_API
#endif