#ifndef GAME
#define GAME

#include <string>

#include "lua_shell.h"
#include "gameinfo.h"
#include "player.h"

class Game {
private:
	Player player;
	GameObject boss;
	LuaScript *current_region;

	std::string filename;
	std::string region;

	bool isComplete;

	void new_game();

public:
	Game(std::string name) : filename(std::string(GAME_FILE) + "_" + name), isComplete(false) {};

	virtual void setRegion(const std::string &_r) { region = _r; }

	virtual void load();
	virtual void save();
	virtual bool pause();
	virtual void play();
};

void playGame(std::string game);
Game *getCurrentGame();

#endif
