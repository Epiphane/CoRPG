#include <iostream>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <sys/stat.h>

#include "js_shell.h"
#include "curl.h"
#include "game.h"
#include "ui.h"

using namespace std;

/* Game info */
Game *Game::instance() {
	static Game *inst = new Game();
	return inst;
}

void Game::load() {
	Window("loading.txt");

	UI::refresh();

	player.fetch();
	player.infoPage("Player stats");

	region = "tavern";
}

void Game::save() {
}

void Game::play() {
	load();

	isComplete = false;
	while (!isComplete) {
		JSRegion current_region = JSRegion("world/" + region + ".js", this);
		if (current_region.error()) break;

		current_region.run();
		if (current_region.error()) break;
	}
}

bool Game::pause() {
	Window pauseScreen("pause.txt");

	char ch = UI::getchar(vector<char>({ '1', '2' }));

	if (ch == '1') {
		return true;
	}
	else {
		save();
		isComplete = true;
		return false;
	}
}

void Game::new_game() {}

GameObject *Game::getObject(const std::string &name) {
	Json::Value info = Curl::GET(region + "/" + name, deps);
	if (info.type() == Json::ValueType::objectValue && info.isMember("name")) {
		return new GameObject(info);
	}
	else { // Not found!
		return NULL;
	}
}

GameObject *Game::getOrBuild(const string &name, Json::Value _default) {
	GameObject *found = getObject(name);
	if (found) return found;

	// Create new object
	Json::Value object;
	object["name"]       = name;
	object["region"]     = region;
	object["isNew"]      = true;
	object["properties"] = _default;

	GameObject *newObj = new GameObject(object);
	newObj->save();

	return newObj;
}
