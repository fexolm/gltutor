#include <cstdio>
#include <unistd.h>
#include "utils.h"
#include <filesystem>

void utils::initGlut(int *argc,
                     char **argv,
                     unsigned int (*defaults)(unsigned int displayMode, int &width, int &height)) {
  glutInit(argc, argv);
  int width = 500;
  int height = 500;

  unsigned int displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;
  displayMode = defaults(displayMode, width, height);

  glutInitDisplayMode(displayMode);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutInitWindowSize(width, height);
  glutInitWindowPosition(300, 200);
  int window = glutCreateWindow(argv[0]);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(-1);
  }
}

void utils::displayFunc(void (*callback)(void)) {
  glutDisplayFunc(callback);
}

void utils::reshapeFunc(void (*callback)(int, int)) {
  glutReshapeFunc(callback);
}

void utils::keyboardFunc(void (*callback)(unsigned char, int, int)) {
  glutKeyboardFunc(callback);
}

void utils::startLoop() {
  glutMainLoop();
}

GLuint utils::createProgram(const std::vector<GLuint> &shaderList) {
  GLuint program = glCreateProgram();

  for (auto shader: shaderList) {
    glAttachShader(program, shader);
  }

  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    auto strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, nullptr, strInfoLog);
    fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    delete[] strInfoLog;
  }

  for (auto shader: shaderList) {
    glDetachShader(program, shader);
  }
  return program;
}

GLuint utils::loadShader(GLenum eShaderType, const std::string &strShaderFilename) {
  auto shaderPath = std::filesystem::current_path();
  shaderPath.append(strShaderFilename);
  std::ifstream shaderFile(shaderPath);
  std::string shaderData((std::istreambuf_iterator<char>(shaderFile)),
                         std::istreambuf_iterator<char>());
  shaderFile.close();

  GLuint shader = glCreateShader(eShaderType);
  const char *rawData = shaderData.c_str();
  glShaderSource(shader, 1, &rawData, nullptr);
  glCompileShader(shader);
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    auto strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog);

    const char *strShaderType = "";
    switch (eShaderType) {
    case GL_VERTEX_SHADER:strShaderType = "vertex";
      break;
    case GL_GEOMETRY_SHADER:strShaderType = "geometry";
      break;
    case GL_FRAGMENT_SHADER:strShaderType = "fragment";
      break;
    }

    fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
    delete[] strInfoLog;
  }

  return shader;
}

