cmake_minimum_required(VERSION 3.23)

project(TutorialMathConsumer LANGUAGES CXX)

set(TUTORIAL_MATH_CONFIG_DIR ""
    CACHE PATH "Path to directory containing TutorialMathConfig.cmake")

if(NOT TUTORIAL_MATH_CONFIG_DIR)
    message(FATAL_ERROR
        "TUTORIAL_MATH_CONFIG_DIR is not set.\n"
        "Please configure with something like:\n"
        "  -DTUTORIAL_MATH_CONFIG_DIR=build/11-install-export-package-ninja/install/lib/cmake/TutorialMath\n"
    )
endif()

set(TutorialMath_DIR "${TUTORIAL_MATH_CONFIG_DIR}"
    CACHE PATH "TutorialMath package config directory" FORCE)

find_package(TutorialMath CONFIG REQUIRED)

add_executable(tutorial_math_consumer
    main.cpp
)

target_link_libraries(tutorial_math_consumer
    PRIVATE
        TutorialMath::tutorial_math
)

target_compile_features(tutorial_math_consumer PRIVATE cxx_std_17)