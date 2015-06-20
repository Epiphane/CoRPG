#ifndef GAME
#define GAME

#include <string>

#include "gameinfo.h"
#include "player.h"

class Game {
private:
	Player player;
	GameObject boss;

	std::string filename;
	std::string region;

	void new_game();

public:
	Game(std::string name) : filename(std::string(GAME_FILE) + "_" + name) {};

	virtual void setRegion(std::string _r) { region = _r; }

	virtual void load();
	virtual void save();
	virtual bool pause();
	virtual void play();
};

void playGame(std::string game);
Game *getCurrentGame();

#endif
