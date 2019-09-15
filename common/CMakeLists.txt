add_library(utils utils.cpp)
target_link_libraries(utils GL GLEW ${GLUT_LIBRARIES} X11)
