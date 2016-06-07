#pragma once
#include <windows.h>	
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "Math\Point3.h"
#include "Math\Point2.h"
#include "Colour.h"

class GameObject
{
public:
	
	float x,y;
	float width, height;
	float halfWidth, halfHeight;
	Color color;
	bool textured;
	bool player;
	bool toDraw;
	GLuint currentTexture;
	
	
	GameObject(void);
	GameObject(Point2<float>); //X Y
	~GameObject(void);

	virtual void setSize(float, float);
	virtual void setColour(enum Color);
	virtual void draw();
	void		 setTexture(GLuint);

	virtual void toString(){
		//
	}

};

