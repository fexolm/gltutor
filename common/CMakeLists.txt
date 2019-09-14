project(common)

add_library(utils utils.cpp)
target_link_libraries(utils OpenGL GLEW ${GLUT_LIBRARIES} X11)
