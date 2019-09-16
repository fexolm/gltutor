cmake_minimum_required(VERSION 3.14)
project(gltest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(GLUT REQUIRED)

include_directories(common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(glutil)
add_subdirectory(triangle)
add_subdirectory(perspective)
add_subdirectory(Overlaping)
add_subdirectory(Translation)
add_subdirectory(World)