#include <cstring>
#include "utils.h"

unsigned int defaults(unsigned int displayMode, int &width, int &height);
void keyboard(unsigned char key, int x, int y);
void reshape(int w, int h);
void display();
void init();

const int numberOfVertices = 36;

#define RIGHT_EXTENT 0.8f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT 0.20f
#define MIDDLE_EXTENT 0.0f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -1.25f
#define REAR_EXTENT -1.75f

#define GREEN_COLOR 0.75f, 0.75f, 1.0f, 1.0f
#define BLUE_COLOR    0.0f, 0.5f, 0.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertexData[] = {
    //Object 1 positions
    LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
    LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
    RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
    RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,

    LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,
    LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
    RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
    RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

    LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
    LEFT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
    LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

    RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,
    RIGHT_EXTENT, MIDDLE_EXTENT, FRONT_EXTENT,
    RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

    LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,
    LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,
    RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT,
    RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,

//	0, 2, 1,
//	3, 2, 0,

    //Object 2 positions
    TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
    MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
    MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
    TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,

    BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
    MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
    MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
    BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

    TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
    MIDDLE_EXTENT, RIGHT_EXTENT, FRONT_EXTENT,
    BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,

    TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,
    MIDDLE_EXTENT, LEFT_EXTENT, FRONT_EXTENT,
    BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

    BOTTOM_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
    TOP_EXTENT, RIGHT_EXTENT, REAR_EXTENT,
    TOP_EXTENT, LEFT_EXTENT, REAR_EXTENT,
    BOTTOM_EXTENT, LEFT_EXTENT, REAR_EXTENT,

    //Object 1 colors
    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,

    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,

    RED_COLOR,
    RED_COLOR,
    RED_COLOR,

    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,

    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,

    //Object 2 colors
    RED_COLOR,
    RED_COLOR,
    RED_COLOR,
    RED_COLOR,

    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,

    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,

    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,

    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,
};

const GLshort indexData[] = {
    0, 2, 1,
    3, 2, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    11, 13, 12,

    14, 16, 15,
    17, 16, 14,
};

GLuint theProgram;

GLuint offsetUniform;
GLuint perspectiveMatrixUnif;

float perspectiveMatrix[16];
const float fFrustumScale = 1.0f;

GLuint vertexBufferObject;
GLuint indexBufferObject;

GLuint vaoObject;

void InitializeProgram() {
  std::vector<GLuint> shaderList;

  shaderList.push_back(utils::loadShader(GL_VERTEX_SHADER, "shaders/vertexShader.glsl"));
  shaderList.push_back(utils::loadShader(GL_FRAGMENT_SHADER, "shaders/fragmentShader.glsl"));

  theProgram = utils::createProgram(shaderList);

  offsetUniform = glGetUniformLocation(theProgram, "offset");

  perspectiveMatrixUnif = glGetUniformLocation(theProgram, "perspectiveMatrix");

  float zNear = 0.5f;
  float zFar = 3.0f;
  memset(perspectiveMatrix, 0, sizeof(float) * 16);

  perspectiveMatrix[0] = fFrustumScale;
  perspectiveMatrix[5] = fFrustumScale;
  perspectiveMatrix[10] = (zFar + zNear) / (zNear - zFar);
  perspectiveMatrix[14] = (2 * zFar * zNear) / (zNear - zFar);
  perspectiveMatrix[11] = -1.0f;

  glUseProgram(theProgram);
  glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, perspectiveMatrix);
  glUseProgram(0);
}

void InitializeVertexBuffer() {
  glGenBuffers(1, &vertexBufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &indexBufferObject);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void InitializeVertexArrayObjects() {
  glGenVertexArrays(1, &vaoObject);
  glBindVertexArray(vaoObject);

  size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) colorDataOffset);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

  glBindVertexArray(0);
}

void init() {
  InitializeProgram();
  InitializeVertexBuffer();
  InitializeVertexArrayObjects();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
  glEnable(GL_DEPTH_CLAMP);
}

void display() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(theProgram);

  glBindVertexArray(vaoObject);

  int count1 = 18, count2 = 18;

  glUniform3f(offsetUniform, 0.0f, 0.0f, 0.9f);
  glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

  glUniform3f(offsetUniform, 0.0f, 0.0f, 0.5f);
  glDrawElementsBaseVertex(GL_TRIANGLES, ARRAY_COUNT(indexData),
                           GL_UNSIGNED_SHORT, 0, numberOfVertices / 2);

  glBindVertexArray(0);
  glUseProgram(0);

  glutSwapBuffers();
}

void reshape(int w, int h) {
  perspectiveMatrix[0] = fFrustumScale * (h / (float) w);
  perspectiveMatrix[5] = fFrustumScale;

  glUseProgram(theProgram);
  glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, perspectiveMatrix);
  glUseProgram(0);

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:glutLeaveMainLoop();
    return;
  }
}

unsigned int defaults(unsigned int displayMode, int &width, int &height) { return displayMode; }

int main(int argc, char **argv) {
  utils::initGlut(&argc, argv, defaults);
  init();
  utils::displayFunc(display);
  utils::reshapeFunc(reshape);
  utils::keyboardFunc(keyboard);
  utils::startLoop();
  return 0;
}
