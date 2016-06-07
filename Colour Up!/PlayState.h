#pragma once

#include "State.h"

class PlayState : public State {

public:

	enum LevelResult {
		END_REACHED,
		PLAYER_DIED
		//time ran out? 
		//suicided? (e.g, ran out of light sources, level reset)
	};

	PlayState(GSM& gsmn/*, level to load*/);
	~PlayState();

	void processKeys(bool *keySet);
	void update(const double dt);
	void render();

	void initLevel();
	void endGame(LevelResult res);

	LevelItem::ObjList objectList; //all objects in scene EXCEPT for the player
	PlayerCharacter *player;
	PlayerCharacter::ActionList playerActions;

	Checkpoint *latestCheckpoint;

};

