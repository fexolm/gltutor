#ifndef GLTEST_COMMON_UTILS_H
#define GLTEST_COMMON_UTILS_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <sstream>
#include <fstream>
#include <vector>

namespace utils {
void initGlut(int *argc, char **argv, unsigned int (*defaults)(unsigned int displayMode, int &width, int &height));
void displayFunc(void (*callback)());
void reshapeFunc(void (*callback)(int, int));
void keyboardFunc(void (*callback)(unsigned char, int, int));
void startLoop();
GLuint loadShader(GLenum eShaderType, const std::string &strShaderFilename);
GLuint createProgram(const std::vector<GLuint> &shaderList);
}
#endif //GLTEST_COMMON_UTILS_H
