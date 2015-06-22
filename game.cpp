#include <iostream>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <sys/stat.h>

#include "main.h"
#include "lua_shell.h"
#include "gameinfo.h"
#include "game.h"

using namespace std;

/* Game info */
Game *currentGame;
Game *getCurrentGame() { return currentGame; }

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

void Game::play() {
	while (!isComplete) {
		Region current_region(region, this);
		if (current_region.error()) break;
		
		current_region.run();
		if (current_region.error()) break;
	}
}

bool Game::pause() {
	ScrBox pauseScreen(30, 20);

	pauseScreen.printlncenter("Paused");
	pauseScreen.println();

	pauseScreen.printlnleft("[1] - Continue");
	pauseScreen.printlnleft("[2] - Save and quit");

	char ch;
	do { ch = pauseScreen.getchar(); } while (ch != '1' && ch != '2');

	if (ch == '1') return true;
	else {
		save();
		isComplete = true;
		return false;
	}
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
	options.println();
	options.println();
	
	options.print("  Enter first boss name: "); 
	boss.name = options.input();
	options.println();

	int suggestedHP = 100;
	options.println();
	options.print("  Suggested: %d", suggestedHP);
	options.println(); options.moveCursor(0, -2);
	options.print("Enter first boss health: ");
	boss.health = atoi(options.input().c_str());
	options.println();
	options.println();
	options.println();
	options.println();

	options.printlncenter("Press Enter to continue...");
	options.input(ALLOW_EMPTY);

	save();
}
