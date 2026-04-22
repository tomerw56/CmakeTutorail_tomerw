cmake_minimum_required(VERSION 3.23)

project(VersioningDemo VERSION 1.2.3 LANGUAGES CXX)

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(GENERATED_HEADER "${GENERATED_DIR}/generated_version.h")

file(MAKE_DIRECTORY "${GENERATED_DIR}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/generated_version.h.in"
    "${GENERATED_HEADER}"
    @ONLY
)

add_executable(versioning_demo
    app/main.cpp
)

target_include_directories(versioning_demo
    PRIVATE
        "${GENERATED_DIR}"
)

target_compile_features(versioning_demo PRIVATE cxx_std_17)