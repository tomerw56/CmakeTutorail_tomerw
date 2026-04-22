cmake_minimum_required(VERSION 3.23)

project(WindowsVersionInfoDemo VERSION 1.4.2 LANGUAGES CXX)

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${GENERATED_DIR}")

set(GENERATED_VERSION_HEADER "${GENERATED_DIR}/generated_version.h")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/generated_version.h.in"
    "${GENERATED_VERSION_HEADER}"
    @ONLY
)

if(WIN32)
    set(APP_VERSION_RC "${GENERATED_DIR}/app_version.rc")
    set(DLL_VERSION_RC "${GENERATED_DIR}/dll_version.rc")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/app_version.rc.in"
        "${APP_VERSION_RC}"
        @ONLY
    )

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/dll_version.rc.in"
        "${DLL_VERSION_RC}"
        @ONLY
    )

    # Harden the sample: explicitly mark generated resource files as RC.
    set_source_files_properties(
        "${APP_VERSION_RC}" "${DLL_VERSION_RC}"
        PROPERTIES LANGUAGE RC
    )
endif()

add_library(demo_dll SHARED
    dll/demo_api.cpp
)

target_include_directories(demo_dll
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/dll"
        "${GENERATED_DIR}"
)

target_compile_definitions(demo_dll
    PRIVATE
        BUILDING_DEMO_DLL
)

target_compile_features(demo_dll PRIVATE cxx_std_17)

if(WIN32)
    target_sources(demo_dll PRIVATE "${DLL_VERSION_RC}")
endif()

add_executable(version_info_app
    app/main.cpp
)

target_link_libraries(version_info_app
    PRIVATE
        demo_dll
)

target_include_directories(version_info_app
    PRIVATE
        "${GENERATED_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/dll"
)

target_compile_features(version_info_app PRIVATE cxx_std_17)

if(WIN32)
    target_sources(version_info_app PRIVATE "${APP_VERSION_RC}")
endif()