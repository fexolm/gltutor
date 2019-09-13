project(tutor1)
find_package(GLUT REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main OpenGL GLEW ${GLUT_LIBRARIES} X11)