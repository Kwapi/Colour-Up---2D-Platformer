#include "ActivityManager.h"


ActivityManager::ActivityManager()
{
	activeState = START;
}


ActivityManager::~ActivityManager()
{
}

void ActivityManager::init(){
	//not sure
}

void ActivityManager::update(){
 	if (!getActiveState()->isOn()){
		switch(activeState){
		case START:
			toGame();
			break;
		case GAME:
			activities[END]->end_score = activities[GAME]->end_score;
			activities[END]->end_time = activities[GAME]->end_time;
			activities[END]->win = activities[GAME]->win;
			toEnd();
		case END:
			if(!getActiveState()->isOn()){
				if(getActiveState()->restart){
					activities[GAME]->loaded = false;
					restart();
					
				}
				else{
					std::exit(0);
				}
			}
		}

		getActiveState()->load();
	}
}


void ActivityManager::add(Activity *activity){
	activities.push_back(activity);
}

void ActivityManager::toStart(){
	activeState = START;
	activities[activeState]->init();
}

void ActivityManager::toGame(){
	activeState = GAME;
	activities[activeState]->init();
}

void ActivityManager::toEnd(){
	activeState = END;
	activities[activeState]->init();
}

void ActivityManager::restart(){
	activities[END]->turnOn();
	activities[END]->restart = false;
	
	toGame();
		
}

Activity* ActivityManager::getActiveState(){
	return activities[activeState];
}



