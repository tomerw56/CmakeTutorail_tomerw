cmake_minimum_required(VERSION 3.23)

project(ConfigurationDemo LANGUAGES CXX)

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES
        "Debug;Release;RelWithDebInfo;MinSizeRel;SimDebug"
        CACHE STRING "" FORCE
    )
endif()

add_executable(config_demo main.cpp)

target_compile_definitions(config_demo PRIVATE
    $<$<CONFIG:Debug>:APP_CONFIG_NAME=\"Debug\">
    $<$<CONFIG:Release>:APP_CONFIG_NAME=\"Release\">
    $<$<CONFIG:RelWithDebInfo>:APP_CONFIG_NAME=\"RelWithDebInfo\">
    $<$<CONFIG:MinSizeRel>:APP_CONFIG_NAME=\"MinSizeRel\">
    $<$<CONFIG:SimDebug>:APP_CONFIG_NAME=\"SimDebug\">

    $<$<CONFIG:SimDebug>:ENABLE_SIMULATION=1>
    $<$<CONFIG:SimDebug>:ENABLE_EXTRA_TRACE=1>
)

target_compile_features(config_demo PRIVATE cxx_std_17)