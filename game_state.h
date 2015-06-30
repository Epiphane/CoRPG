#ifndef GAME_STATE_H
#define GAME_STATE_H 

#include "gameinfo.h"

class LocalGameState {
private:
	GameObject player;

public:
	LocalGameState();

	GameObject *getPlayer() { return &player; }
};

#endif /* GAME_STATE_H */
