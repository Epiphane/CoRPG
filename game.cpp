#include <iostream>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <sys/stat.h>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include "LuaBridge/Source/LuaBridge/LuaBridge.h"

#include "main.h"
#include "gameinfo.h"
#include "game.h"

using namespace std;

/* Game info */
Game *currentGame;

void createNewGame(std::string game);

void playGame(std::string name) {	
	currentGame = new Game(name);
	currentGame->load();
	currentGame->play();
}

void Game::load() {
	ScrBox loading(18, 5);
	loading.printlnleft("Loading...");
	refreshScreen();
	loading.printlnleft("Loaded!");

	// Read game file
	fstream game_file(filename.c_str(), iostream::in);
	if (!game_file.is_open()) {
		new_game();

		game_file.open(filename.c_str(), iostream::in);
	}

	stringstream file_stream;
	read_file(game_file, file_stream);

	file_stream >> region;

	player.load(file_stream);
	boss.load(file_stream);
	refreshScreen();
}

void Game::save() {
	ScrBox saving(18, 5);
	saving.printlnleft("Saving...");
	refreshScreen();
	saving.printlnleft("Saved!");

	stringstream file;
	file << region << endl;
	player.save(file);
	boss.save(file);
	
	fstream game(filename.c_str(), iostream::out);
	if (!game.is_open()) {
		cout << "Error opening " << filename << endl;
	}

	write_file(game, file);

	game.close();
	refreshScreen();
}

void lua_clear() {

}

void lua_print(const char *format, ...) {

}

void Game::play() {
	lua_State* L = luaL_newstate();
	luaL_dofile(L, (region + ".lua").c_str());
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);
//	luabridge::LuaRef s = getGlobal(L, "welcome");
}

void Game::new_game() {
	fstream game(filename.c_str(), iostream::in);
	if (game.is_open()) {
		cout << "Game Already Exists" << endl;
		game.close();

		return;
	}

	region = "tavern";
	player.health = 100;

	clearScreen();

	ScrBox options(60, 20);

	options.println();

	options.print("      Enter world name: ");
	boss.name = options.input();
	options.println();
	
	options.print("      Enter world name: ");
	boss.name = options.input();
	options.println();

	options.print("     Enter player name: ");
	player.name = options.input();
	options.println();

	options.println();
	options.print("  Suggested: lavender");
	options.println(); options.moveCursor(0, -2);
	options.print("Enter player hair color: ");
	player.setProperty("hair", options.input());
	options.println();
	
	options.print("  Enter first boss name: "); 
	boss.name = options.input();

	int suggestedHP = 100;
	options.println();
	options.print("  Suggested: %d", suggestedHP);
	options.println(); options.moveCursor(0, -2);
	options.print("Enter first boss health: ");
	boss.health = atoi(options.input().c_str());
	options.println();

	options.printlncenter("Press Enter to continue...");
	options.input(ALLOW_EMPTY);

	save();
}
