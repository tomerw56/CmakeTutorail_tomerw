cmake_minimum_required(VERSION 3.23)

project(GeneratedFilesDemo LANGUAGES CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(GENERATED_HEADER "${GENERATED_DIR}/generated_build_info.h")

add_custom_command(
    OUTPUT "${GENERATED_HEADER}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${GENERATED_DIR}"
    COMMAND Python3::Interpreter
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/make_generated_header.py"
            "${GENERATED_HEADER}"
            "generated-by-python-script"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tools/make_generated_header.py"
    VERBATIM
)

add_executable(generated_files_demo
    app/main.cpp
    "${GENERATED_HEADER}"
)

target_include_directories(generated_files_demo
    PRIVATE
        "${GENERATED_DIR}"
)

target_compile_features(generated_files_demo PRIVATE cxx_std_17)