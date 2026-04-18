#pragma once

namespace math_headers
{
    inline const char* build_mode_name()
    {
#ifdef NDEBUG
        return "Release-style build";
#else
        return "Debug-style build";
#endif
    }
}