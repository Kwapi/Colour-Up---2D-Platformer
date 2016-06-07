#include "CollidableObject.h"
#include "GameObject.h"
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "BoundingBox.h"
#include "Math\Point2.h"
#include <iostream>


CollidableObject::CollidableObject(void){
	this->c = Circle(0, 0, 0);
}

void CollidableObject::toString(){
	std::cout<<"Collidable"<<std::endl;
}

CollidableObject::CollidableObject(Point2<float> dimensions, Point2<float> coordinates, PlatformType plType) : GameObject(coordinates){
	
	this->platformType = plType;
	blended = false;
	this->bB = BoundingBox(dimensions.x, dimensions.y);

	toRemove = false;
	setSize(dimensions.x, dimensions.y);

	//set the cooordinates for this
	this->x = coordinates.x;
	this->y = coordinates.y;

	x_original = x;
	y_original = y;
	//create a boundingCircle
	double radius = getHypotenuse(bB.width, bB.height) / 2;
	this->c = Circle(x, y, radius);

	//set the coordinates for the bounding box
	this->bB.y = y;
	this->bB.x = x;
	reverseSpeed =  false;
	 timeElapsed = 0;
	 motionDuration = 0;
	 speed = 0;
	 moveRange = 0;
	 toDraw = true;
	 toCollide = true;
}

CollidableObject::CollidableObject(Point2<float> dimensions, Point2<float> coordinates) : GameObject(coordinates){
	toRemove = false;
	this->bB = BoundingBox(dimensions.x, dimensions.y);
	blended = false;
	setSize(dimensions.x, dimensions.y);
	
	//create a boundingCircle
	double radius = getHypotenuse(bB.width, bB.height) / 2;
	this->c = Circle(x, y, radius);
	c.setColour(GREEN);

	//set the coordinates for the bounding box
	this->bB.y = y;
	this->bB.x = x;

	
	x_original = x;
	y_original = y;
	 reverseSpeed =  false;
	 timeElapsed = 0;
	 motionDuration = 0;
	 	 speed = 0;

		 toDraw = true;
		 toCollide = true;
}

CollidableObject::~CollidableObject()
{

}

bool CollidableObject::collide(CollidableObject & other){
	
	return bB.collide(other.bB); 
}

void CollidableObject:: drawBounding(){
	
	glPushMatrix();
	glTranslatef(x,y,0); 
	this->bB.draw();
	this->c.draw();

	glPopMatrix();
	

}


void CollidableObject::update(){ 
	this->bB.updateCorners();
}

void CollidableObject::checkBlending(Color c){

	blended = (color==c) ? true : false;

}


void CollidableObject::move(double dt){
	
	float posXlimit;
	float negXlimit;
	
	switch(platformType){
	case ALPHAFLOOR:
		 speed = dt * speedMod*2.5;
		 this->y += speed;
		 this->bB.y += speed;
		break;
		case MOVINGY:
			
			speed = dt * speedMod;
			timeElapsed +=dt;
			
			
			
			if (timeElapsed>motionDuration ){
				if(reverseSpeed){
					reverseSpeed = false;
				}
				else{
					reverseSpeed = true;
				}
				timeElapsed = 0;
			}

			if(reverseSpeed){
				speed = speed * -1;
						
			}
			
	

			//apply movement
			this->y += speed;
			this->bB.y += speed;

			break;
		case MOVINGX:
			
			speed = dt * speedMod;
			timeElapsed +=dt;
			
			
			
			if (timeElapsed>motionDuration ){
				if(reverseSpeed){
					reverseSpeed = false;
				}
				else{
					reverseSpeed = true;
				}
				timeElapsed = 0;
			}

			if(reverseSpeed){
				speed = speed * -1;
						
			}
			
	

			//apply movement
			this->x += speed;
			this->bB.x += speed;

			break;
		case ENEMY:
			speed = speedMod * dt;

			
		    posXlimit = x_original + moveRange;
			negXlimit = x_original - moveRange;

			if (this->x >= posXlimit || this->x <= negXlimit){
				if (reverseSpeed){
					reverseSpeed = false;
					changeTexture(reverseSpeed);
				}
				else{
					reverseSpeed = true;
					changeTexture(reverseSpeed);
				}
		    }

			if (reverseSpeed){
				speed = speed * -1;
				
							

			}
			

			//apply movement
			this->x += speed;
			this->bB.x += speed;
			break;
		default:
			//
			break;
	}
}

void CollidableObject::setMotionDuration(float motion){
	this->motionDuration = motion;
}

void CollidableObject::setSpeedMod(int mod){
	this->speedMod = mod;
}

void CollidableObject::tieNPCtoPlatform(CollidableObject &platform){
	float newX = platform.x;

	this->x = newX;
	this->bB.x = this->x;
	this->x_original = this->x;

	boolean posValue = platform.y >= 0;
	float newY = platform.y + this->height / 2 + platform.height / 2;
	this->y = newY;
	this->bB.y = newY;

	this->moveRange = (platform.width / 2) - this->width / 2;

	
}

void CollidableObject::stopDisplaying(){
	toRemove = true;
}


void CollidableObject::changeTexture(bool left){
	if (left)
		currentTexture = textures[1];
	else{
		currentTexture = textures[0];
	}
}