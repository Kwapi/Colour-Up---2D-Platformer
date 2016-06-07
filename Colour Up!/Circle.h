#pragma once


#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "Math/Point4.h"
#include "GameObject.h"
#include "Math\Vector2.h"

class BoundingBox;

class Circle : public GameObject{

public:
	static const double PI;
	Circle(void);
	Circle(double, double, double);
	bool contains(const double x,const double y) const;
	bool contains(const Vector2f& pos) const;
	
	bool intersects(const Circle& other) const;
	bool intersects(const BoundingBox& box) const;
	void draw(/*texturetomap*/) const;
	~Circle(void);
	

private:
	double radius;

};

