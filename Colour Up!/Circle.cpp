#include "Circle.h"

//#include "Image_Loading/nvImage.h"

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include "BoundingBox.h"

const double Circle::PI = 3.1415926535897932384626433;

Circle::Circle(void)
{
}

Circle::Circle(double x, double y, double radius){
	this->x = x;
	this->y = y;
	this->radius = radius;
	color = GREEN;
}
bool Circle::contains(const double x,const double y) const {
	return contains(Vector2f(x,y)); 
}

bool Circle::contains(const Vector2f& pos) const {
	return (x - pos.x)*(x - pos.x) + (y - pos.y)*(y - pos.y) < radius*radius;
}


bool Circle::intersects(const BoundingBox& box) const {
	double xDist = abs(x - box.x);
	double yDist = abs(y - box.y);

	if (xDist > (box.width / 2 + radius)) { return false; }
	if (yDist > (box.height / 2 + radius)) { return false; }

	if (xDist <= (box.width / 2)) { return true; }
	if (yDist <= (box.height / 2)) { return true; }

	double cornDistsq = (xDist - box.width / 2) * (xDist - box.width / 2)
				      + (yDist - box.height / 2) * (yDist - box.height / 2);
				
	return cornDistsq <= (radius * radius);
}

bool Circle::intersects(const Circle& other) const {
	return ((x - other.x)*(x - other.x) +        //return (difference in x position)
		(y - other.y)*(y - other.y) <			 //plus (difference in y position)
		(radius + other.radius)*(radius + other.radius)); //is less than (combined radius)
}

void Circle::draw(/*texturetomap*/) const {

	glPushMatrix();
	glColor3fv(getColorGL(color));
	glBegin(GL_LINE_LOOP);
		for(int i=0; i<360; i+=5)
			glVertex2f( radius * cos(i*(PI/180)), 
					    radius * sin(i*(PI/180)));
	glEnd();
	glPopMatrix();
}

Circle::~Circle(void)
{
}