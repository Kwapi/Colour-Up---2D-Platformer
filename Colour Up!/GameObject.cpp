#include "GameObject.h"
#include <windows.h>	
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "Colour.h"
#include <iostream>
#include "Player.h"

//-----CONSTRUCTORS----//

//default
GameObject::GameObject(void)
{
}

//Position
GameObject:: GameObject(Point2<float> coordinates){
	this->x = coordinates.x;
	this->y = coordinates.y;

	width = 20;			//default dimensions
	height = 20;
	color = WHITE;		//BLACK BY DEFAULT
	
	//no textures by default
	textured = false;
	currentTexture=0;
	player = false;
	toDraw = true;
	
}



//Destructor
GameObject::~GameObject(void)
{
}



//-----METHODS-----//

void GameObject:: setSize(float w, float h){
	this->width = w;
	this->height = h;
}
void GameObject:: setColour(Color c){
	this->color = c;
}

void GameObject:: draw(){
	halfWidth = width/2;
	halfHeight = height/2;
	glPushMatrix();
	
	glTranslatef(x,y,0);
	//the object is textured
	if (textured){
			
		glColor3fv(getColorGL(color));
		glEnable(GL_TEXTURE_2D);
		
		//if it's a player don't do color overlay

		GLfloat param = (player) ? GL_REPLACE : GL_MODULATE; 

		glEnable(GL_BLEND);	
		glBindTexture(GL_TEXTURE_2D, currentTexture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );

		int tiles = (int)width / height;

		if (width<height){
			tiles =(int) height / width;


			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0); glVertex2f(-halfWidth, -halfHeight);
			glTexCoord2f(0.0, tiles); glVertex2f(-halfWidth, halfHeight);
			glTexCoord2f(1, tiles); glVertex2f(halfWidth, halfHeight);
			glTexCoord2f(1, 0); glVertex2f(halfWidth, -halfHeight);
			glEnd();
		}
		else{

			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0); glVertex2f(-halfWidth, -halfHeight);
			glTexCoord2f(0.0, 1.0); glVertex2f(-halfWidth, halfHeight);
			glTexCoord2f(tiles, 1.0); glVertex2f(halfWidth, halfHeight);
			glTexCoord2f(tiles, 0.0); glVertex2f(halfWidth, -halfHeight);
			glEnd();
		}
		
		
		


		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
	else{
		glColor3fv(getColorGL(color));
		glBegin(GL_POLYGON);
			glVertex2f(-halfWidth, -halfHeight);
			glVertex2f(-halfWidth, halfHeight);
			glVertex2f(halfWidth, halfHeight);
			glVertex2f(halfWidth, -halfHeight);
		glEnd();

	}
	

	glPopMatrix();
}

void GameObject::setTexture(GLuint textureID){
	textured = true;
	this->currentTexture = textureID;
}


