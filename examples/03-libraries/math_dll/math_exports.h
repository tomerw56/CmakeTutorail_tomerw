#pragma once

#ifdef _WIN32
    #ifdef BUILDING_MATH_DLL
        #define MATH_API __declspec(dllexport)
    #else
        #define MATH_API __declspec(dllimport)
    #endif
#else
    #define MATH_API
#endif