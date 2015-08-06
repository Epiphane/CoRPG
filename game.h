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
	std::unordered_map<std::string, GameObject *> cache;
	std::vector<std::string> deps;

	std::string region;
	Json::Value region_args;

	bool isComplete;

	void new_game();

	Game() : player("player", "global"), isComplete(true) { deps.clear(); cache.clear(); };

public:
	static Game *instance();

	GameObject *getPlayer() { return &player; }
	GameObject *getObject(const std::string &name);
	GameObject *getOrBuild(const std::string &name, Json::Value _default);

	virtual void setRegion(const std::string &_r, Json::Value args) { deps.clear(); region = _r; region_args = args; }
	std::string getRegion() { return region; }
	Json::Value getRegionArgs() { return region_args; }
	void addDependency(const std::string region) { deps.push_back(region); }

	virtual void load();
	virtual void save();
	virtual bool pause();
	virtual void play();
};

#endif
