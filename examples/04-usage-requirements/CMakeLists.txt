cmake_minimum_required(VERSION 3.23)

project(UsageRequirementsDemo LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Header-only library to demonstrate INTERFACE usage requirements
# ---------------------------------------------------------------------------
add_library(math_headers INTERFACE)

target_include_directories(math_headers
    INTERFACE
        math_headers/include
)

target_compile_features(math_headers
    INTERFACE
        cxx_std_17
)

target_compile_definitions(math_headers
    INTERFACE
        MATH_HEADERS_ENABLED=1
)

# ---------------------------------------------------------------------------
# Compiled library to demonstrate PUBLIC and PRIVATE usage requirements
# ---------------------------------------------------------------------------
add_library(core_math
    core_math/src/math_api.cpp
)

target_include_directories(core_math
    PUBLIC
        core_math/include
    PRIVATE
        core_math/src
)

target_compile_definitions(core_math
    PUBLIC
        CORE_MATH_API_LEVEL=2
    PRIVATE
        CORE_MATH_INTERNAL_BUILD=1
)

target_link_libraries(core_math
    PUBLIC
        math_headers
)

# ---------------------------------------------------------------------------
# Application consuming the library
# ---------------------------------------------------------------------------
add_executable(usage_requirements_demo
    app/main.cpp
)

target_link_libraries(usage_requirements_demo
    PRIVATE
        core_math
)