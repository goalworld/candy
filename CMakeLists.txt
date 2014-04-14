cmake_minimum_required (VERSION 2.8)
include (CheckIncludeFiles)
include (CheckSymbolExists)
include (CheckCSourceCompiles)

project (candy C)
enable_testing ()

#  Platform checks.

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions (-DCANDY_HAVE_WINDOWS)
else ()
    message (FATAL_ERROR "ERROR: CMake build system is intended only to generate MSVC solution files.\n" )
endif ()

add_subdirectory (./src)


#  Build unit tests.

set (all_tests "")

macro (add_libnanomsg_test NAME)
    list (APPEND all_tests ${NAME})
    add_executable (${NAME} tests/${NAME}.c)
    target_link_libraries (${NAME} nanomsg)
    add_test (${NAME} ${NAME})
endmacro (add_libnanomsg_test)


