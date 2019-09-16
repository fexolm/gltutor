add_library(glutil MatrixStack.cpp MousePoles.cpp Shader.cpp WindowPos.cpp)
target_link_libraries(glutil GL GLEW ${GLUT_LIBRARIES} X11 utils)