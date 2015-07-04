#ifndef GAME
#define GAME

#include <string>
#include <vector>
#include <unordered_map>

#include "gameinfo.h"
#include "json/json.h"

class Game {
private:
	GameObject player;
	std::unordered_map<std::string, GameObject *> objects;
	std::vector<std::string> deps;

	std::string region;

	bool isComplete;

	void new_game();

	Game() : player("player", "global"), isComplete(false) { objects.clear(); };

public:
	static Game *instance();

	GameObject *getPlayer() { return &player; }
	GameObject *getObject(const std::string &name);
	GameObject *newObject(const std::string &name);

	virtual void setRegion(const std::string &_r) { region = _r; }
	std::string getRegion() { return region; }
	void addDependency(const std::string region) { deps.push_back(region); }

	virtual void load();
	virtual void save();
	virtual bool pause();
	virtual void play();
};

#endif
