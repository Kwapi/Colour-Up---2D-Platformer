#pragma once

#include "KeyboardDefinitions.h";	// Header File for Keyboard Definitions
#include <windows.h>				// Header File For Windows
#include "Image_Loading/nvImage.h"
#include <gl\gl.h>					// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "GameObject.h"
#include "BoundingBox.h"
#include "CollidableObject.h"
#include "Math/Point2.h"
#include "Math/Point4.h"
#include "Player.h"
#include "console.h"				//Header File for Console	
#include "Colour.h"
#include "Clock.h"
#include <vector>
#include "freetype.h"		// Header for font library.


#include "ImageLoading.h"
class Activity
{
public:
	Activity();
	~Activity();
	//stores state of keys
	bool keys[256];
	//left mouse click
	bool LMBPressed;
	//mouse pointer coordinates
	Point2<int> mouseXY;

	enum ActivityState{
		ON, OFF
	};

	ActivityState activityState;

	void load()
	{
		if(!loaded)
		{
			init();
			loaded = true;
		}
	}
	virtual void init();
	virtual void draw();
	virtual void update(const double dt);
	virtual void updateInput();
	int					screenWidth;
	int					screenHeight;
	void turnOn();
	void turnOff();
	bool isOn();
	bool	restart;
	bool loaded;
	freetype::font_data our_font;
	freetype::font_data our_font2;
	bool win;
	int end_score;
	int end_time;
};

