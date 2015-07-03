#include <iostream>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <sys/stat.h>

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

	UI::getchar();
}

void Game::save() {
}

void Game::play() {
	load();

	cout << "Sup" << endl;
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
	return NULL;
}

GameObject *Game::newObject(const std::string &name) {
	return NULL;
}
