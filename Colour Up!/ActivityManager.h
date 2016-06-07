#pragma once
#include <stack>

#include "Activity.h"
class ActivityManager
{
public:
	ActivityManager();
	~ActivityManager();

	enum State{ START, GAME, END};
	
	std::vector<Activity*> activities;

	State activeState;

	void init();
	
	void update();
	
	void add(Activity*);
	void toStart();
	void toGame();
	void toEnd();
	void restart();
	Activity* getActiveState();






	
};

