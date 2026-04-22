cmake_minimum_required(VERSION 3.23)

project(TutorialMath VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# -----------------------------------------------------------------------------
# Library
# -----------------------------------------------------------------------------
add_library(tutorial_math
    src/math_utils.cpp
)

add_library(TutorialMath::tutorial_math ALIAS tutorial_math)

target_include_directories(tutorial_math
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(tutorial_math PUBLIC cxx_std_17)

set_target_properties(tutorial_math PROPERTIES
    EXPORT_NAME tutorial_math
)

# -----------------------------------------------------------------------------
# Install targets and headers
# -----------------------------------------------------------------------------
install(TARGETS tutorial_math
    EXPORT TutorialMathTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# -----------------------------------------------------------------------------
# Package config files
# -----------------------------------------------------------------------------
set(TUTORIAL_MATH_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/TutorialMath"
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TutorialMathConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TutorialMathConfig.cmake"
    INSTALL_DESTINATION "${TUTORIAL_MATH_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TutorialMathConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT TutorialMathTargets
    FILE TutorialMathTargets.cmake
    NAMESPACE TutorialMath::
    DESTINATION "${TUTORIAL_MATH_INSTALL_CMAKEDIR}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TutorialMathConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TutorialMathConfigVersion.cmake"
    DESTINATION "${TUTORIAL_MATH_INSTALL_CMAKEDIR}"
)

# -----------------------------------------------------------------------------
# Basic CPack setup
# -----------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "TutorialMath")
set(CPACK_PACKAGE_VENDOR "CMake Tutorial")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)