#pragma once
#include "Activity.h"
class EndGame:
	public Activity
{
public:
	EndGame(void);
	~EndGame(void);
	void draw();
	void init();
	void update(const double dt);
	void updateInput();
	void restartGame();

	
	GameObject endScreen;
};

