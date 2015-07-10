#ifndef GAME
#define GAME

#include <string>
#include <vector>
#include <unordered_map>

#include "gameinfo.h"
#include "js_shell.h"
#include "json/json.h"

class Game {
private:
	GameObject player;
	std::vector<std::string> deps;

	std::string region;

	bool isComplete;

	void new_game();

	Game() : player("player", "global"), isComplete(true) {};

public:
	static Game *instance();

	GameObject *getPlayer() { return &player; }
	GameObject *getObject(const std::string &name);
	GameObject *getOrBuild(const std::string &name, Json::Value _default);

	virtual void setRegion(const std::string &_r) { region = _r; }
	std::string getRegion() { return region; }
	void addDependency(const std::string region) { deps.push_back(region); }

	virtual void load();
	virtual void save();
	virtual bool pause();
	virtual void play();
};

#endif
