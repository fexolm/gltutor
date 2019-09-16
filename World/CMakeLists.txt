include(common)

add_executable(world main.cpp Mesh.cpp)
target_link_libraries(world GL GLEW ${GLUT_LIBRARIES} X11 utils stdc++fs glutil)

COPY_SHADERS(world)
COPY_MESHES(world)
