cmake_minimum_required(VERSION 3.18)

project(hw05)

# =========================================
# DO NOT EDIT LINES BELOW lest you break it
# =========================================


# If you add a new source to your project, just run ‹cmake -B build -S .›
# or rerun CMake configuration from your IDE.


# Main executable
# ---------------

file(GLOB SOURCES "*.c")
set(EXECUTABLE dt)
set(FEATURE_MACROS -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE)
add_executable(${EXECUTABLE} ${SOURCES})
target_compile_definitions(${EXECUTABLE} PUBLIC ${FEATURE_MACROS})
target_link_libraries(${EXECUTABLE} PUBLIC "-lm")


# Unit Test Files
# ---------------

file(GLOB TESTS_CUT "tests-cut/tests-*.c")
set(LIB_CUT cutobj)

add_library(${LIB_CUT} ${SOURCES})
target_compile_definitions(${LIB_CUT} PUBLIC ${FEATURE_MACROS})

set(EXE_CUT ${EXECUTABLE}-tests-cut)
add_executable(${EXE_CUT} ${TESTS_CUT} "tests-cut/cut-main.c")
target_include_directories(${EXE_CUT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${EXE_CUT} PUBLIC ${FEATURE_MACROS})
target_link_libraries(${EXE_CUT} ${LIB_CUT} "-lm")


# CLI Test Files
file(GLOB TESTS_CLI_SH "tests-cli/*.sh")
file(GLOB TESTS_CLI_DATA "tests-cli/data/*")

file(COPY ${TESTS_CLI_SH} "tests-cli/dt-tests-cli" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "tests-cli/data/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)


# Configure compiler warnings
if (CMAKE_C_COMPILER_ID MATCHES Clang OR ${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    # Using regular Clang, AppleClang or GCC
    # Strongly suggested: able -Werror
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic -g -Og")
elseif (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
    # Using Visual Studio C++
    target_compile_definitions(${EXECUTABLE} PRIVATE _CRT_SECURE_NO_DEPRECATE)
    target_compile_definitions(${EXECUTABLE_TESTS} PRIVATE _CRT_SECURE_NO_DEPRECATE)
    set(CMAKE_CXX_FLAGS "/permissive- /W4 /EHsc")
endif ()

if (MINGW)
    target_compile_definitions(${EXECUTABLE} PRIVATE __USE_MINGW_ANSI_STDIO=1)
    target_compile_definitions(${EXECUTABLE_TESTS} PRIVATE _CRT_SECURE_NO_DEPRECATE)
endif ()
