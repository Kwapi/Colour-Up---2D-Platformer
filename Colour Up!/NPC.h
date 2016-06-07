#pragma once
#include "CollidableObject.h"
#include "Math\Point2.h"
class BoundingBox;
class Player : public CollidableObject 
{
public:
	enum Action{
		RUNL, RUNR, IDLE, JUMPUP, JUMPDOWN, SHOOT, KICK, FUCK
	};

	//	true if player is alive
	bool alive;

	//	number of lives
	int livesCount;

	// True if currently jumping (prevents the player jumping while in mid-air)
	bool jumping;

	// The amount of X acceleration to apply when the player moves left or right
	// The amount of X deceleration to apply when the player does not move left or right
	float accX, decX;
	float accY, decY;

	// The maximum movement speed allowed for the player
	float maxSpeedX, maxSpeedY;

	// The amount of upward force to apply when the player first presses jump
	float jumpStartSpeedY;

	//flag allowing player to jump (you have to release button before jumping again)
	bool allowedToJump;

	// The amount of X and Y movement to occur on the next frame
	float newSpeedX, newSpeedY;

	// Current speed from the previous frame
	float currentSpeedX, currentSpeedY;

	// Move request flags from keyboard
	bool  moveRequestRight, moveRequestLeft, moveRequestUp, moveRequestDown;

	//	value of gravity
	float gravity;
	
	//	Flags marking from which side player is colliding
	bool contactBottom, contactTop, contactRight, contactLeft;

	GLuint textures[4];
	
	

	Player(void);
	~Player(void);

	Player(Point2<float>, Point2<float>);
	Player(BoundingBox&,Point2<float>,  Point4<GLfloat> );
	
	
	void move(double);
	void getNewSpeed(double);
	void modifySpeed(float, float, float, float, double);
	void reset(Point2f checkpoint);
	
	bool collide(CollidableObject&);
	void resetRequests();
	void initialise();
	void die();
	void changeTexture(enum Action);
	void translate(float, float, double);

	void toString();
};

