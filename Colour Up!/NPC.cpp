#include "Player.h"
#include "CollidableObject.h"
#include "GameObject.h"
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "BoundingBox.h"
#include "Math\Point2.h"
#include "Maths.h"
#include "Colour.h"
#include <iostream>
#include <cmath>


Player::Player(void)
{
}

void Player::toString(){
	std::cout<<"PLAYER"<<std::endl;
}


Player::~Player(void)
{
}

//Bounding Box + X and Y 
Player::Player(Point2<float> dimensions, Point2<float> coordinates) : CollidableObject(dimensions, coordinates){
	initialise();
	player = true;	
}

//Calculate new speed based on the player input and gravity
//This speed will then be checked using collision detection and modified to prevent the collision
//Then newSpeed will become currentSpeed

void Player::getNewSpeed(double dt){

	//new speed is based on current speed and then gets modified
	newSpeedX = currentSpeedX;
	newSpeedY = currentSpeedY;

	//apply gravity
	newSpeedY += gravity;

	//DOWN KEY PRESSED
	if (moveRequestDown){
		//todo: CROUCH
		newSpeedY -= accY;
	}

	//LEFT KEY PRESSED 
	if (moveRequestLeft){
		newSpeedX -= accX;
	}
	
	//RIGHT KEY PRESSED 
	if (moveRequestRight){
		newSpeedX += accX;
	}
	
	//JUMP
	//player can't be in the process of jumping and has to be touching the floor (hence speed checks)
	if (moveRequestUp && !jumping && (currentSpeedY > -15 && currentSpeedY < 15)){
		//jump
		jumping = true;
		newSpeedY += jumpStartSpeedY;
	}	
	
		
	// Limit the sideways acceleration of the player
	if (newSpeedX > maxSpeedX) newSpeedX = maxSpeedX;
	if (newSpeedX < -maxSpeedX) newSpeedX = -maxSpeedX;

	/*
	OLD CODE MIGHT BE USEFUL LATER

	// Limit the force of gravity (terminal velocity)
	if (newSpeedY > maxSpeedY) newSpeedY = maxSpeedY;
	if (newSpeedY < -maxSpeedY) newSpeedY = -maxSpeedY;
	*/
	
	
	// Decelerate the player's sideways movement if left or right wasn't pressed
	if ((!moveRequestLeft && !moveRequestRight))
	{
		if (newSpeedX < 0) newSpeedX += decX;
		if (newSpeedX > 0) newSpeedX -= decX;
		

		// Deceleration may produce a speed that is greater than zero but
		// smaller than the smallest unit of deceleration. These lines ensure
		// that the player does not keep travelling at slow speed forever after
		// decelerating.
		if (newSpeedX > 0 && newSpeedX < decX) newSpeedX = 0;
		if (newSpeedX < 0 && newSpeedX > -decX) newSpeedX = 0;
	}

	Action action;
	if(moveRequestLeft){
		action = RUNL;
	}else if(moveRequestRight){
		action = RUNR;
	}else{
		action = IDLE;
	}

	changeTexture(action);
	

	
}


void Player::modifySpeed(float pushDistRight, float pushDistLeft, float pushDistDown, float pushDistUp, double dt){

		
	//FIND THE SMALLEST DISTANCE SO THAT THE PLAYER DOESN'T COLLIDE
	//CORRECT THE VELOCITY BY THAT DISTANCE
	
	//PLAYER HAS TO BE PUSHED RIGHT (ADD TO VELOCITY)
	if (pushDistRight<pushDistLeft && pushDistRight<pushDistDown && pushDistRight<pushDistUp){
		newSpeedX += pushDistRight/dt;
	}
	//PLAYER HAS TO BE PUSHED LEFT (SUBTRACT FROM VELOCITY)
	else if (pushDistLeft<pushDistRight && pushDistLeft< pushDistDown && pushDistLeft<pushDistUp){
		newSpeedX -= pushDistLeft/dt;
	}
	//PLAYER HAS TO BE PUSHED DOWN (SUBTRACT FROM VELOCITY)
	else if (pushDistDown<pushDistRight && pushDistDown<pushDistLeft && pushDistDown<pushDistUp){
		newSpeedY -= pushDistDown/dt;
	}
	//PLAYER HAS TO BE PUSHED DOWN (SUBTRACT FROM VELOCITY)
	else if (pushDistUp<pushDistRight && pushDistUp<pushDistLeft && pushDistUp<pushDistDown){
		newSpeedY += pushDistUp/dt;
		jumping = false;				//PLAYER HITS THE BOTTOM SO THEY ARE NOT JUMPING ANYMORE
	}

	//OLD CODE (MIGHT BE USEFUL LATER)

	/*
	TAKING INTO CONSIDERATION CONTACT FLAGS
	if (contactLeft){
		newSpeedX += pushDistRight/dt;
	}
	if (contactRight){
		newSpeedX -= pushDistLeft/dt;
	}
	if (contactTop){
		newSpeedY -= pushDistDown/dt;
	}
	*/
	//if (contactBottom){
		//jumping = false;
	//}
}

/*	MOVE
*	(TRANSLATE PLAYER, TRANSLATE BOUNDING BOX, SET CURRENTSPEED TO MODIFIED NEWSPEED
*/
void Player::move(double dt){

	//Translate player
	x += newSpeedX * dt;
	y += newSpeedY * dt;

	//Translate bounding box
	bB.translate(newSpeedX, newSpeedY, dt);

	//Make newSpeed currentSpeed
	currentSpeedX = newSpeedX;
	currentSpeedY = newSpeedY;
}


//OVERRIDEN
bool Player::collide(CollidableObject& other){
	
	return this->bB.collide(other.bB);


}

void Player::resetRequests(){
	this->moveRequestDown = false;
	this->moveRequestUp = false;
	this->moveRequestLeft = false; 
	this->moveRequestRight = false;
	this->allowedToJump = true;
}


void Player::initialise(){
	float mScale = 12.0f;

	accX = 0.8f * mScale;
	decX = 0.4f * mScale;
	accY = 0.2f * mScale;
	decY = 0.3f * mScale;

	maxSpeedX = 8.0f * mScale;
	maxSpeedY = 10.0f * mScale;

	jumpStartSpeedY = 20.f * mScale;

	gravity = -0.6f * mScale;

	newSpeedX = currentSpeedX =  0.f;
	newSpeedY = currentSpeedY  = 0.f;

	jumping = false;
	blended = false;

	alive = true;
	livesCount = 3;

	resetRequests();
		
	}


void Player::die(){
	alive = false;
	livesCount--;
}

//TRANSLATION
void Player::translate(float speedX, float speedY, double dt){
	x = speedX * dt;
	y = speedY * dt;
	
}

void Player::reset(Point2f checkpoint){
	resetRequests();
	newSpeedX = currentSpeedX = 0.f;
	newSpeedY = currentSpeedY = 0.f;
	alive = true;

	x = checkpoint.x;
	y = checkpoint.y;

	bB.x = x;
	bB.y = y;


}


void Player::changeTexture(Action action){
	switch(action){
	case IDLE:
		currentTexture = textures[0];
		break;
	case RUNL:
		currentTexture = textures[1];
		break;
	case RUNR:
		currentTexture = textures[2];
		break;
	}
}




