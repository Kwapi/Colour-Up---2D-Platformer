#include "StartGame.h"




StartGame::StartGame()
{
	turnOn();
}


StartGame::~StartGame()
{
}


void StartGame::init(){
	setBGColour(BLACK);
	//initialise all keys to false
	for (int i = 0; i < 256; i++){
		keys[i] = false;
	}


	startScreen = GameObject(Point2f(0, 0));
	startScreen.setSize(200, 200);
	startScreen.setTexture(loadPNG("start.png"));
	startScreen.player = true;		//fix
	
}

void StartGame::update(const double dt){
	updateInput();
}

void StartGame::draw(){
	displayBG();
	glLoadIdentity();
		startScreen.draw();
	glFlush();
}

void StartGame::updateInput(){
	//pressing space takes user to next state (PLAY)
	if (keys[VK_SPACE]){
			turnOff();
	}

}
