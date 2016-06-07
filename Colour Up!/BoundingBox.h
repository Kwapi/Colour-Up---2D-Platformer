
#pragma once
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "Math/Point4.h"
#include "GameObject.h"
#include "Math\Point2.h"


class BoundingBox : public GameObject
{
	
public:
	float minX, minY, maxX, maxY; 
	bool collision;
	BoundingBox(void);
	BoundingBox(float, float);
	~BoundingBox(void);

	bool collide(BoundingBox&);
	bool contains(const double x, const double y) const;
	bool contains(const Point2f& pos) const;

	void translate(float, float, double);
	
	void updateCorners(void);

	void draw();
};

