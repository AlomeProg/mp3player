cmake_minimum_required(VERSION 3.5)
project(mp3player)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -static")
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(raylib raylib/bin)
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(raygui/projects/CMake)

file(GLOB_RECURSE SOURCE_MP3PLAYER src/main.cpp)

add_executable(mp3player ${SOURCE_MP3PLAYER})

target_link_libraries(mp3player raylib raygui)
target_include_directories(mp3player PRIVATE include/)
