#pragma once
#include "GameObject.h"

#include "BoundingBox.h"
#include "Circle.h"
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "Math/Point2.h"
#include "Math/Point4.h"
#include "Maths.h"

class CollidableObject : public GameObject
{
public:
	
	enum PlatformType{
		PLATFORM, DEADLYPLATFORM, MOVINGX, MOVINGY, ENEMY, LAMBDA, CMYK, ALPHAFLOOR, HSV
	};

	BoundingBox bB;
	Circle c;
	
	float x_original,y_original;
	bool blended;
	bool toRemove;
	float speed;
	int speedMod;
	float motionDuration;
	float moveRange;
	bool reverseSpeed;
	float timeElapsed;
	bool toCollide;
	PlatformType platformType;
	GLuint textures[2];
	CollidableObject(void);
	/*
	CollidableObject(BoundingBox,Point2<float>);
	*/
	CollidableObject(Point2<float>, Point2<float>, PlatformType);
	CollidableObject(Point2<float>, Point2<float>);

	~CollidableObject(void);

	virtual bool collide(CollidableObject&);
	void changeTexture(bool);
	void tieNPCtoPlatform(CollidableObject&);

	void updateCorners(void);

	void drawBounding(void);

	void update(void);

	void checkBlending(enum Color);

	void toString();

	void move(double);

	void setMotionDuration(float);
	void setSpeedMod(int);

	void stopDisplaying();
	
	
	
	
};

