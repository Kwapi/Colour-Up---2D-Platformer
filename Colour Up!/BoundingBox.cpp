#include "BoundingBox.h"
#include "Math/Point4.h"
#include "Colour.h"



BoundingBox::BoundingBox(void)
{
}
BoundingBox::BoundingBox(float width, float height){
	color = GREEN; //grey by default
	collision = false;
	
	this->width = width;
	this->height = height;

	//used for drawing
	this->halfWidth = (width)/2;
	this->halfHeight = (height)/2; 



	
	
}

BoundingBox::~BoundingBox(void)
{
}


void BoundingBox::draw()
{	
	glPushMatrix();
	glColor3fv(getColorGL(color)); //colour the bounding outline
	  
	glBegin(GL_LINE_LOOP);
		glVertex2f(-halfWidth,-halfHeight);	//left bottom
		glVertex2f(-halfWidth,halfHeight);	//left top
		glVertex2f(halfWidth,halfHeight);	//right top
		glVertex2f(halfWidth,-halfHeight);	//right bottom
	glEnd();
	glPopMatrix();
}

//Colission detection using AABB
bool BoundingBox::collide(BoundingBox& other){
	

	collision = false; 

	this->updateCorners();		//update corners of both bounding boxes
	other.updateCorners();
	
	
	//CHECK FOR COLLISION
		if(this->minX> other.maxX) return false;

		if(this->minY> other.maxY) return false;

		if(this->maxX < other.minX) return false; 

		if(this->maxY < other.minY) return false;   
	
	
		
	collision = true;
	return true;

}




void BoundingBox:: updateCorners(){

	this->minX = -halfWidth + x;		//bottom left corner X
	this->minY = -halfHeight + y;		//bottom left corner Y
	this->maxX =  halfWidth + x;		//top right corner X
	this->maxY =  halfHeight + y;		//top right corner Y
	
}


void BoundingBox::translate(float speedX, float speedY, double dt){
	x += speedX * dt;
	y += speedY * dt;

	updateCorners();
}
bool BoundingBox::contains(const double x1, const double y1) const {
	return x1 >= x - (width / 2)
		&& x1 <= x + (width / 2)
		&& y1 >= y - (height / 2)
		&& y1 <= y + (height / 2);
}

bool BoundingBox::contains(const Point2f& pos) const {
	return contains(pos.x, pos.y);
}
