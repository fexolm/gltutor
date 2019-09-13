#include "../common/utils.h"

#include <algorithm>
#include <cmath>

unsigned int defaults(unsigned int displayMode, int &width, int &height);

void keyboard(unsigned char key, int x, int y);

void reshape(int w, int h);

void display();

void init();

GLuint theProgram;

const std::vector<float> vertexPositions = {
    0.0f,    0.5f, 0.0f, 1.0f,
    0.5f, -0.366f, 0.0f, 1.0f,
    -0.5f, -0.366f, 0.0f, 1.0f,
    1.0f,    0.0f, 0.0f, 1.0f,
    0.0f,    1.0f, 0.0f, 1.0f,
    0.0f,    0.0f, 1.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;

void computePositionOffsets(float &fXOffset, float &fYOffset) {
  const float fLoopDuration = 5.0f;
  const float fScale = 3.14159f * 2.0f / fLoopDuration;

  float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

  float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

  fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.5f;
  fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.5f;
}

void adjustVertexData(float fXOffset, float fYOffset) {
  std::vector<float> fNewData(vertexPositions);

  for(int v = 0; v < fNewData.size(); v++) {
    fNewData[v] += fXOffset;
    fNewData[v + 1] += fYOffset;
  }

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), fNewData.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InitializeProgram() {
  std::vector<GLuint> shaderList;

  shaderList.push_back(utils::loadShader(GL_VERTEX_SHADER, "shaders/vertexShader.glsl"));
  shaderList.push_back(utils::loadShader(GL_FRAGMENT_SHADER, "shaders/fragmentShader.glsl"));

  theProgram = utils::createProgram(shaderList);

  std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}


void InitializeVertexBuffer() {
  glGenBuffers(1, &positionBufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexPositions.size(), vertexPositions.data(), GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init() {
  InitializeProgram();
  InitializeVertexBuffer();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display() {
  float fXOffset = 0.0f, fYOffset = 0.0f;
  computePositionOffsets(fXOffset, fYOffset);
  adjustVertexData(fXOffset, fYOffset);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(theProgram);

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glUseProgram(0);

  glutSwapBuffers();
  glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to
//exit the program.
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
