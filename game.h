#ifndef GAME
#define GAME

#include <string>
#include <vector>
#include <unordered_map>

#include "lua_shell.h"
#include "gameinfo.h"

class Game {
private:
	GameObject player;
	std::unordered_map<std::string, GameObject *> objects;
	LuaScript *current_region;

	std::vector<std::string> deps;

	std::string filename;
	std::string region, name;

	bool isComplete;

	void new_game();

public:
	Game(std::string name) : player("game_player"), filename(std::string(GAME_FILE) + "_" + name), isComplete(false) { objects.clear(); };

	GameObject *getPlayer() { return &player; }

	GameObject *getObject(const std::string &name);
	GameObject *newObject(const std::string &name);

	virtual void setRegion(const std::string &_r) { region = _r; }
	std::string getRegion() { return region; }
	std::string getName() { return name; }
	void addDependency(const std::string region) { deps.push_back(region); }

	virtual void load();
	virtual void save();
	virtual bool pause();
	virtual void play();
};

void playGame(std::string game);
Game *getCurrentGame();

#endif
