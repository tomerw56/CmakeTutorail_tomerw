#include <iostream>
#ifdef NDEBUG
#include "conditional_depnded.h"
#endif
#ifndef APP_CONFIG_NAME
#define APP_CONFIG_NAME "Unknown"
#endif

int main()
{
    std::cout << "Configuration name: " << APP_CONFIG_NAME << "\n";

#ifdef NDEBUG
    std::cout << "NDEBUG: defined (release-style assertions disabled)\n";
	depended_opration();
#else
    std::cout << "NDEBUG: not defined (debug-style assertions enabled)\n";
#endif

#ifdef ENABLE_SIMULATION
    std::cout << "Simulation mode: ENABLED\n";
#else
    std::cout << "Simulation mode: disabled\n";
#endif

#ifdef ENABLE_EXTRA_TRACE
    std::cout << "Extra trace: ENABLED\n";
#else
    std::cout << "Extra trace: disabled\n";
#endif

    return 0;
}