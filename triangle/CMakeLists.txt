project(tutor1)

add_executable(tutor1 main.cpp)
target_link_libraries(tutor1 OpenGL GLEW ${GLUT_LIBRARIES} X11 utils stdc++fs)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR}/triangle)