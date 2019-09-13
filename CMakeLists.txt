cmake_minimum_required(VERSION 3.14)
project(gltest)

set(CMAKE_CXX_STANDARD 17)

find_package(GLUT REQUIRED)

add_library(utils common/utils.cpp)

target_link_libraries(utils OpenGL GLEW ${GLUT_LIBRARIES} X11)

add_subdirectory(triangle)
