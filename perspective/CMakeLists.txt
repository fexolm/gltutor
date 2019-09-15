include(common)

add_executable(perspective main.cpp)
target_link_libraries(perspective GL GLEW ${GLUT_LIBRARIES} X11 utils stdc++fs)

COPY_SHADERS(perspective)
