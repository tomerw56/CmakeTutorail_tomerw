cmake_minimum_required(VERSION 3.23)

project(DynamicLoadingDemo LANGUAGES CXX)

add_library(math_dll SHARED
    math_dll/math_api.cpp
)

target_include_directories(math_dll PRIVATE math_dll)

target_compile_definitions(math_dll PRIVATE BUILDING_MATH_DLL)

add_executable(dll_loader
    app/main.cpp
)

add_custom_command(TARGET dll_loader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:math_dll>
            $<TARGET_FILE_DIR:dll_loader>
)