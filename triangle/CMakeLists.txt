include(common)

add_executable(triangle main.cpp)
target_link_libraries(triangle OpenGL GLEW ${GLUT_LIBRARIES} X11 utils stdc++fs)

COPY_SHADERS(triangle)




