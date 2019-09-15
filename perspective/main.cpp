#include <cstring>
#include "utils.h"

unsigned int defaults(unsigned int displayMode, int &width, int &height);
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);
void display();
void init();

GLuint theProgram;
GLuint positionBufferObject;
GLuint vao;
GLuint loopTime;
GLuint offsetUniform;
GLuint perspectiveMatrixUniform;

float perspectiveMatrix[16];
float frustumScale;

const std::vector<float> vertexPositions = {
    0.25f, 0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,

    0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,

    0.25f, 0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,

    -0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,

    -0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,

    0.25f, 0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,

    0.25f, 0.25f, -1.25f, 1.0f,
    0.25f, 0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,

    0.25f, 0.25f, -2.75f, 1.0f,
    0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,

    0.25f, 0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
};

void InitializeProgram() {
  std::vector<GLuint> shaderList;

  shaderList.push_back(utils::loadShader(GL_VERTEX_SHADER, "shaders/vertexShader.glsl"));
  shaderList.push_back(utils::loadShader(GL_FRAGMENT_SHADER, "shaders/fragmentShader.glsl"));

  theProgram = utils::createProgram(shaderList);

  offsetUniform = glGetUniformLocation(theProgram, "offset");

  perspectiveMatrixUniform = glGetUniformLocation(theProgram, "perspectiveMatrix");

  frustumScale = 1.0f;
  float zNear = 0.5f;
  float zFar = 3.0f;
  memset(perspectiveMatrix, 0, sizeof(float) * 16);

  perspectiveMatrix[0] = frustumScale;
  perspectiveMatrix[5] = frustumScale;
  perspectiveMatrix[10] = (zFar + zNear) / (zNear - zFar);
  perspectiveMatrix[14] = (2 * zFar * zNear) / (zNear - zFar);
  perspectiveMatrix[11] = -1.0f;

  glUseProgram(theProgram);
  glUniformMatrix4fv(perspectiveMatrixUniform, 1, GL_FALSE, perspectiveMatrix);
  glUseProgram(0);
}

void InitializeVertexBuffer() {
  glGenBuffers(1, &positionBufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexPositions.size(), vertexPositions.data(), GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init() {
  InitializeProgram();
  InitializeVertexBuffer();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
}

void reshape(int w, int h) {
  perspectiveMatrix[0] = frustumScale / ((float) w / (float) h);
  perspectiveMatrix[5] = frustumScale;

  glUseProgram(theProgram);
  glUniformMatrix4fv(perspectiveMatrixUniform, 1, GL_FALSE, perspectiveMatrix);
  glUseProgram(0);
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void display() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(theProgram);

  glUniform2f(offsetUniform, 0.5f, 0.5f);

  size_t colorData = vertexPositions.size() * sizeof(float) / 2;

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) colorData);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glUseProgram(0);

  glutSwapBuffers();
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:glutLeaveMainLoop();
    return;
  }
}

unsigned int defaults(unsigned int displayMode, int &width, int &height) {
  return displayMode;
}

int main(int argc, char **argv) {
  utils::initGlut(&argc, argv, defaults);
  init();
  utils::displayFunc(display);
  utils::reshapeFunc(reshape);
  utils::keyboardFunc(keyboard);
  utils::startLoop();
  return 0;
}
