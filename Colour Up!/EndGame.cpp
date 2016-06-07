#include "EndGame.h"


EndGame::EndGame(void)
{
	turnOn();
	restart = false;
}


EndGame::~EndGame(void)
{
}


void EndGame::init(){
	setBGColour(BLACK);
	//initialise all keys to false
	for (int i = 0; i < 256; i++){
		keys[i] = false;
	}

	our_font.init("pier.otf", 40);
	our_font2.init("pier.otf", 22);
	endScreen.player = true;		//fix
	
}

void EndGame::update(const double dt){
	updateInput();
}

void EndGame::draw(){
	displayBG();
	glLoadIdentity();
	glPushMatrix();
	if (win){
		freetype::print(our_font, screenWidth / 2.0 - screenWidth / 10, screenHeight / 2 + screenHeight / 8, "YOU WON\n\nSCORE: %d \nTIME: %d s \nTOTAL SCORE: %d", end_score, end_time, end_score + (200 - end_time) * 100);
		freetype::print(our_font2, screenWidth / 2.0 - screenWidth / 10, screenHeight / 2 - screenHeight / 5, "PRESS SPACEBAR TO START AGAIN");
	}
	else{
		freetype::print(our_font, (screenWidth / 2.0) - screenWidth/10, screenHeight / 2 + screenHeight/10, "GAME OVER\n\nSCORE: %d", end_score);
		freetype::print(our_font2, (screenWidth / 2.0) - screenWidth / 10, screenHeight / 2 - screenHeight/10, "PRESS SPACEBAR TO START AGAIN");
	}
	
	glPopMatrix();
	glFlush();
}

void EndGame::updateInput(){
	//pressing space takegs user to next state (PLAY)
	if (keys[VK_SPACE]){
			restartGame();
	}
	if (keys[VK_E]){
		turnOff();
	}

}

void EndGame::restartGame(){
	turnOff();
	restart = true;
	keys[VK_SPACE] = false;
}
