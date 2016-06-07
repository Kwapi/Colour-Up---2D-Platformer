#include "Activity.h"


Activity::Activity()
{
	LMBPressed = false;
	loaded = false;
	win = false;
}


Activity::~Activity()
{
}

void Activity::turnOff(){
	activityState = OFF;
}

void Activity::turnOn(){
	activityState = ON;
}

bool Activity::isOn(){
	switch (activityState){
	case OFF:
		return false;
		break;
	case ON:
		return true;
		break;
	}
}


void Activity::init(){
	//
}
void Activity::draw(){
	//
}
void Activity::update(const double dt){
	//
}

void Activity::updateInput(){
	//
}
