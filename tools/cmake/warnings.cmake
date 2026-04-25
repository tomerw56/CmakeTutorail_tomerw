# cmake/warnings.cmake
option(ENABLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
function(enable_project_warnings target_name)
    if(NOT TARGET "${target_name}")
        message(FATAL_ERROR "enable_project_warnings: target '${target_name}' does not exist")
    endif()
    if(MSVC)
        target_compile_options(${target_name} PRIVATE
            /W4
            /permissive-
        )
    else()
        target_compile_options(${target_name} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()
    if(ENABLE_WARNINGS_AS_ERRORS)
        if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
            set_property(TARGET ${target_name} PROPERTY COMPILE_WARNING_AS_ERROR ON)
        else()
            if(MSVC)
                target_compile_options(${target_name} PRIVATE /WX)
            else()
                target_compile_options(${target_name} PRIVATE -Werror)
            endif()
        endif()
    endif()
endfunction()