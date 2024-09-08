cmake_minimum_required(VERSION 3.0)

# Project configuration
project(hw02)
set(SOURCES main.c)
set(EXECUTABLE poker)
set(EXECUTABLE_TESTS poker-tests)

# Test files
file(GLOB TESTS_GLOB "tests/*.c")

# Test utility files
file(GLOB TESTS_LIBS_GLOB "tests/libs/*.c")

set(TEST_SOURCES ${TESTS_LIBS_GLOB} ${TESTS_GLOB})

# Executables
add_executable(${EXECUTABLE_TESTS} ${TEST_SOURCES})
add_executable(${EXECUTABLE} ${SOURCES})

# Configure compiler warnings
if (CMAKE_C_COMPILER_ID MATCHES Clang OR ${CMAKE_C_COMPILER_ID} STREQUAL GNU)
  # using regular Clang, AppleClang or GCC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic")
elseif (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
  # using Visual Studio C++
  target_compile_definitions(${EXECUTABLE} PRIVATE _CRT_SECURE_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "/permissive- /W4 /EHsc")
endif()

if(MINGW)
    target_compile_definitions(${EXECUTABLE} PRIVATE __USE_MINGW_ANSI_STDIO=1)
	target_compile_definitions(${EXECUTABLE_TESTS} PRIVATE __USE_MINGW_ANSI_STDIO=1)
endif()
