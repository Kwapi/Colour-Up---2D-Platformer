#pragma once
#include <windows.h>				// Header File For Windows
#include <gl\gl.h>					// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "Colour.h"
#include "Math\Point3.h"
#include <iostream>



//the colors correspond to enums
//i.e. BLUE = 3 => colorArray[3] => c_blue = {0,0,1)
 void setBGColour(Color c){
	currentBGColor = c;
	
}



 void resetBGColour(){
	 currentBGColor = WHITE;
 }

void displayBG(){
	
	//SET
	glClearColor(colorArray[currentBGColor][0],colorArray[currentBGColor][1],colorArray[currentBGColor][2],1);
	glClear(GL_COLOR_BUFFER_BIT);	//DISPLAY

}

const GLfloat* getColorGL(Color c) {
	return colorArray[c];
}
