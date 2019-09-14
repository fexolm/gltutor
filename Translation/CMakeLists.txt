include(common)

add_executable(translation main.cpp)
target_link_libraries(translation OpenGL GLEW ${GLUT_LIBRARIES} X11 utils stdc++fs)

COPY_SHADERS(translation)
