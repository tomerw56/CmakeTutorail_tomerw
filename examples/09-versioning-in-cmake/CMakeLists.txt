cmake_minimum_required(VERSION 3.23)

set(VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/version.txt")
file(STRINGS "${VERSION_FILE}" PROJECT_SEMVER LIMIT_COUNT 1)

project(VersioningDemo VERSION ${PROJECT_SEMVER} LANGUAGES CXX)

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

add_custom_target(bump_major
    COMMAND ${CMAKE_COMMAND}
            -DVERSION_FILE=${VERSION_FILE}
            -DMODE=major
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bump_version.cmake"
    COMMENT "Bump major version and reset minor/patch to zero"
)

add_custom_target(bump_minor
    COMMAND ${CMAKE_COMMAND}
            -DVERSION_FILE=${VERSION_FILE}
            -DMODE=minor
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bump_version.cmake"
    COMMENT "Bump minor version and reset patch to zero"
)

add_custom_target(bump_patch
    COMMAND ${CMAKE_COMMAND}
            -DVERSION_FILE=${VERSION_FILE}
            -DMODE=patch
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bump_version.cmake"
    COMMENT "Bump patch version"
)