#pragma once
#include "Activity.h"

class StartGame :
	public Activity
{
public:
	StartGame();
	~StartGame();
	void draw();
	void init();
	void update(const double dt);
	void updateInput();


	GameObject startScreen;
};

