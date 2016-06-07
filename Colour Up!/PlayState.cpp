#include <iostream>

#include "GSM.h"
#include "PlayState.h"

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library

#include "Platform.h"
#include "NonPlayerCharacter.h"
#include "Pickup.h"
#include "PlayerCharacter.h"
#include "Checkpoint.h"

PlayState::PlayState(GSM& gsmn/*, level to load*/) : 
State(gsmn)
{
	initLevel();
}


PlayState::~PlayState()
{
	for (LevelItem::ObjListConstIter obj = objectList.begin(); obj < objectList.end(); obj++)
		delete *obj;

	delete player;
}

void PlayState::processKeys(bool *keySet) {
	if (keySet[VK_UP])
		playerActions.push_back(PlayerCharacter::JUMP);

	if (keySet[VK_DOWN])
		playerActions.push_back(PlayerCharacter::CROUCH);
	else
		playerActions.push_back(PlayerCharacter::UNCROUCH);

	if (keySet[VK_LEFT])
		playerActions.push_back(PlayerCharacter::MOVE_LEFT);

	if (keySet[VK_RIGHT])
		playerActions.push_back(PlayerCharacter::MOVE_RIGHT);
}


void PlayState::update(const double dt) {

	for (LevelItem::ObjListConstIter obj = objectList.begin(); obj < objectList.end(); obj++)
		(*obj)->update(dt);

	player->collectActions(playerActions);
	player->update(dt);
	playerActions.clear();

	//iterating through objectList, removing items maraked for removal, whilst not invalidating iterator
	for (LevelItem::ObjListConstIter obj = objectList.begin(); obj != objectList.end(); /*empty*/) {
		if ((*obj)->markedForRemoval)
			obj = objectList.erase(obj);
		else 
			++obj;
	}

	if (player->markedForRemoval)
		endGame(PLAYER_DIED);

	for (LevelItem::ObjListConstIter obj = objectList.begin(); obj < objectList.end(); obj++) {
		Checkpoint* cp = dynamic_cast<Checkpoint*>(*obj);
		if (cp && cp->cptype == Checkpoint::LEVEL_END && cp->activated)
			endGame(END_REACHED);
	}
}

void PlayState::render() {
	for (LevelItem::ObjListConstIter obj = objectList.begin(); obj < objectList.end(); obj++)
		(*obj)->draw();

	player->draw();
}




void PlayState::initLevel() {
	int screenWidth = *getContext().screenWidth;
	int screenHeight = *getContext().screenHeight;

	std::cout << screenWidth << ", " << screenHeight << std::endl;

	Box platformBB1(0.5*screenWidth, 0.1*screenHeight, 
					1.0*screenWidth, 0.2*screenHeight);
	Box platformBB2(0.05*screenWidth, 0.3*screenHeight, 
					0.1*screenWidth, 0.2*screenHeight);

	/*Platform platform1(objectList, platformBB1);
	Platform platform2(objectList, platformBB2);

	PlayerCharacter player(objectList, Vector2f(0.5*(*getContext().screenWidth), 0.5*(*getContext().screenHeight)));
	Pickup pickup(objectList, Vector2f(0.2*(*getContext().screenWidth), 0.2*(*getContext().screenHeight) + Pickup::PICKUP_SIZE), Pickup::HEALTH_FULL);
	Checkpoint checkpoint(objectList, Vector2f(1.0*(*getContext().screenWidth) - 25, 0.2*(*getContext().screenHeight) + 50), Checkpoint::LEVEL_END);
	NonPlayerCharacter npc(objectList, Vector2f(0.8*(*getContext().screenWidth), 0.5*(*getContext().screenHeight)))*/;

	/*objectList.push_back(new PlayerCharacter(objectList, 
		Vector2f(0.5*(*getContext().screenWidth), 
		0.5*(*getContext().screenHeight))));*/

	player = new PlayerCharacter(objectList,
		Vector2f(0.0*screenWidth + 25, 0.8*screenHeight));

	objectList.push_back(new Platform(objectList, platformBB2));
	objectList.push_back(new Platform(objectList, platformBB1));

	//objectList.push_back(new Platform(objectList, platformBB1));
	//objectList.push_back(new Platform(objectList, platformBB2));

	objectList.push_back(new Pickup(objectList, 
		Vector2f(0.2*screenWidth, 0.2*screenHeight + Pickup::PICKUP_SIZE), 
		Pickup::HEALTH_FULL));

	objectList.push_back(new NonPlayerCharacter(objectList, 
		Vector2f(0.8*screenWidth, 0.5*screenHeight)));

	objectList.push_back(new Checkpoint(objectList, 
		Vector2f(1.0*screenWidth - 25, 0.2*screenHeight + 50), 
		Checkpoint::LEVEL_END));

	latestCheckpoint = new Checkpoint(objectList,
		Vector2f(0.0*screenWidth + 25, 0.4*screenHeight + 50),
		Checkpoint::LEVEL_START);

	objectList.push_back(latestCheckpoint);
}

void PlayState::endGame(LevelResult res) {
	switch (res) {
		case END_REACHED:
			gsm.setState(new PlayState(gsm));
			break;
		case PLAYER_DIED:
			gsm.setState(new PlayState(gsm));
			break;
	}
}