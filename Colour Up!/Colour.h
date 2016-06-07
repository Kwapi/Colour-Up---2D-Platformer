#pragma once
#include "Math\Point3.h"
#include <map>
#include <vector>

static int currentBGColor;
const GLfloat colorArray[9][3] = {{1.0f,0.0f,0.0f}, {0,1,0}, {0,0,1}, {0,0,0}, {1,1,1}, {0,1,1}, {1, 0, 1}, {1,1,0}};
enum Color{RED, GREEN, BLUE,BLACK, WHITE, CYAN,MAGENTA,YELLOW};

 void setBGColour(enum Color);
 void displayBG();
 void resetBGColour();

 const GLfloat* getColorGL(Color);


