cmake_minimum_required(VERSION 3.23)

project(GoogleTestLocalArchiveDemo LANGUAGES CXX)

include(FetchContent)

# -----------------------------------------------------------------------------
# Local GoogleTest archive configuration
# -----------------------------------------------------------------------------
set(
    GTEST_ARCHIVE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-1.17.0.tar.gz"
    CACHE FILEPATH
    "Path to a local GoogleTest archive"
)

if(NOT EXISTS "${GTEST_ARCHIVE}")
    message(FATAL_ERROR
        "Local GoogleTest archive not found.\n"
        "Expected path:\n"
        "  ${GTEST_ARCHIVE}\n\n"
        "Please do one of the following:\n"
        "  1. Copy googletest-1.17.0.tar.gz into:\n"
        "     ${CMAKE_CURRENT_SOURCE_DIR}/third_party/\n"
        "  2. Or configure with:\n"
        "     -DGTEST_ARCHIVE=<full path to googletest-1.17.0.tar.gz>\n"
    )
endif()

message(STATUS "Using local GoogleTest archive: ${GTEST_ARCHIVE}")

FetchContent_Declare(
    googletest
    URL "${GTEST_ARCHIVE}"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows / MSVC compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# -----------------------------------------------------------------------------
# Library under test
# -----------------------------------------------------------------------------
add_library(core_math
    src/math_utils.cpp
)

target_include_directories(core_math
    PUBLIC
        src
)

target_compile_features(core_math PRIVATE cxx_std_17)

# -----------------------------------------------------------------------------
# GoogleTest-based test executable
# -----------------------------------------------------------------------------
add_executable(core_math_gtests
    tests/test_math.cpp
)

target_link_libraries(core_math_gtests
    PRIVATE
        core_math
        GTest::gtest_main
)

target_compile_features(core_math_gtests PRIVATE cxx_std_17)

# -----------------------------------------------------------------------------
# Register with CTest
# -----------------------------------------------------------------------------
add_test(NAME core_math_gtests COMMAND core_math_gtests)