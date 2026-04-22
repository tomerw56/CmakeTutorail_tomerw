if(NOT DEFINED VERSION_FILE)
    message(FATAL_ERROR "VERSION_FILE is required")
endif()

if(NOT DEFINED MODE)
    message(FATAL_ERROR "MODE must be one of: major, minor, patch")
endif()

file(STRINGS "${VERSION_FILE}" CURRENT_VERSION LIMIT_COUNT 1)

if(NOT CURRENT_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)$")
    message(FATAL_ERROR
        "Version file must contain MAJOR.MINOR.PATCH, got: ${CURRENT_VERSION}")
endif()

set(MAJOR "${CMAKE_MATCH_1}")
set(MINOR "${CMAKE_MATCH_2}")
set(PATCH "${CMAKE_MATCH_3}")

if(MODE STREQUAL "major")
    math(EXPR MAJOR "${MAJOR} + 1")
    set(MINOR 0)
    set(PATCH 0)
elseif(MODE STREQUAL "minor")
    math(EXPR MINOR "${MINOR} + 1")
    set(PATCH 0)
elseif(MODE STREQUAL "patch")
    math(EXPR PATCH "${PATCH} + 1")
else()
    message(FATAL_ERROR "Unknown MODE: ${MODE}")
endif()

set(NEW_VERSION "${MAJOR}.${MINOR}.${PATCH}")
file(WRITE "${VERSION_FILE}" "${NEW_VERSION}\n")

message(STATUS "Version updated: ${CURRENT_VERSION} -> ${NEW_VERSION}")
message(STATUS "Re-run CMake configure so project(VERSION ...) picks up the change.")