cmake_minimum_required(VERSION 3.23)

project(CTestDemo LANGUAGES CXX)

enable_testing()

add_library(core_math
    src/math_utils.cpp
)

target_include_directories(core_math
    PUBLIC
        src
)

target_compile_features(core_math PRIVATE cxx_std_17)

add_executable(core_math_tests
    tests/test_math.cpp
)

target_link_libraries(core_math_tests
    PRIVATE
        core_math
)

target_compile_features(core_math_tests PRIVATE cxx_std_17)

add_test(NAME core_math_tests COMMAND core_math_tests)