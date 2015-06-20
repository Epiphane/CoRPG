#include <iostream>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <sys/stat.h>

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
	printBox(18, 4);
	print(-1, "Loading...");
	refreshScreen();
	print(0, "Loaded!   ");

	// Read game file
	fstream game_file(filename.c_str(), iostream::in);
	if (!game_file.is_open()) {
		new_game();

		game_file.open(filename.c_str(), iostream::in);
	}

	stringstream file_stream;
	read_file(game_file, file_stream);

	player.load(file_stream);
	boss.load(file_stream);
	refreshScreen();
}

void Game::save() {
	printBox(18, 4);
	print(-1, "Saving...");
	refreshScreen();
	print(0, "Saved!   ");

	stringstream file;
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
	player.infoPage("Player information");
}

void Game::new_game() {
	fstream game(filename.c_str(), iostream::in);
	if (game.is_open()) {
		cout << "Game Already Exists" << endl;
		game.close();

		return;
	}

	player.health = 100;

	clearScreen();
	printBox(60, 20);

	int row = -8;
	print(row, "       Enter world name:                        ");
	boss.name = input(row++, 2);
	row++;

	print(row, "      Enter player name:                        ");
	player.name = input(row++, 2);
	row++;

	print(row, "Enter player hair color:                        ");
	print(row + 1, "Suggested: lavender                           ");
	player.setProperty("hair", input(row++, 2));
	row += 2;

	print(row, "  Enter first boss name:                        ");
	boss.name = input(row++, 2);
	row++;

	int suggestedHP = 100;
	print(row, "Enter first boss health:                        ");
	print(row + 1, "Suggested: %d                               ", suggestedHP);
	boss.health = atoi(input(row++, 2).c_str());
	row += 2;

	print(row, "Press Enter to continue...");
	input(row, 13, ALLOW_EMPTY);

	save();
}
