#include <iostream>
#include <fstream>
#include <ncurses.h>

#include "main.h"
#include "game.h"

using namespace std;
void splash();
void helpScreen();
std::string gameSelectScreen();

int rows, cols;
WINDOW *scr;

int getRows() { return rows; }
int getCols() { return cols; }

int main(int argc, char *argv[]) {
	scr = initscr();								/* Start curses mode 		  */
	noecho();
	getmaxyx(stdscr,rows,cols);		/* get the number of rows and columns */
beginGame:
	splash();

	char choice;
	do {
		choice = getch();
	} while (choice != ' ' && choice != 'h' && choice != 'q' && choice != 27);

	if (choice == ' ') {
		std::string game = gameSelectScreen();

		playGame(game);
	}
	else if (choice == 'h') {
		helpScreen();

		do {
			choice = getch();
		} while (choice != ' ' && choice != 'q');

		if (choice != 'q')
			goto beginGame;
	}

	endwin();								/* End curses mode		  */
}

void clearScreen() { clear(); }
void refreshScreen() { refresh(); }

void splash() {
	clearScreen();

	ScrBox splashBox(48, 12);
	splashBox.printlncenter("* WeeblyRPG *");
	splashBox.println();
	splashBox.printlncenter("By Thomas Steinke");
	splashBox.println();
	splashBox.printlncenter("(space) - Play");
	splashBox.printlncenter("(h)     - Help");
	splashBox.printlncenter("(q)     - Quit");

	refreshScreen();
}

void helpScreen() {
	clearScreen();

	ScrBox helpBox(50, 20);

	helpBox.printlnleft("WeeblyRPG is a text-based RPG where");
	helpBox.printlnleft("everyone plays the same character!");
	helpBox.println();
	helpBox.printlnleft("Work together to strengthen the hero,");
	helpBox.printlnleft("and defeat the boss to add your name");
	helpBox.printlnleft("to the trophy room!");
	helpBox.println();
	helpBox.printlnleft("Be careful, though:");
	helpBox.printlnleft("If someone beats the monster before");
	helpBox.printlnleft("you, they get the fame and you get");
	helpBox.printlnleft("...");
	helpBox.printlnleft("NOTHING!!");
	helpBox.println();
	helpBox.printlnleft("(space) - Go back");
	helpBox.printlnleft("(q) - Quit       ");

	refreshScreen();
}

std::string gameSelectScreen() {
	clearScreen();

	ScrBox gameSelectBox(50, 11);

	gameSelectBox.printlncenter("* Enter Game Name *");
	gameSelectBox.printlncenter("Press (enter) to start");
	gameSelectBox.printlncenter("Leave blank for default Weebly game");
	gameSelectBox.println();
	gameSelectBox.moveCursor(3, 0);

	refreshScreen();
	return gameSelectBox.input(ALLOW_EMPTY);
}

std::string input(int cursor_row, int cursor_col, int flags) {
	move(cursor_row + rows / 2, cursor_col + cols / 2);
	refresh();

	std::string output = "";
	char ch = getch();
	while (ch != '\n' || (output.size() == 0 && !(flags & ALLOW_EMPTY))) {
		if (ch == 127 && output.size() > 0) { // Backspace
			output = output.substr(0, output.size() - 1);

			int cury, curx;
			getyx(scr, cury, curx);

			printw(" ");
			move(cury, curx - 1);
			printw(" ");
			move(cury, curx - 1);
		}
		else if (ch != 127 && ch != '\n') {
			output += ch;
			printw("%c", ch);
		}
		refresh();

		ch = getch();
	}

	return output;
}
