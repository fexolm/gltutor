include(common)

add_executable(overlaping main.cpp)
target_link_libraries(overlaping OpenGL GLEW ${GLUT_LIBRARIES} X11 utils stdc++fs)

COPY_SHADERS(overlaping)
