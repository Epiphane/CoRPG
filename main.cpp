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

int main(int argc, char *argv[]) {
	scr = initscr();								/* Start curses mode 		  */
	noecho();
	getmaxyx(stdscr,rows,cols);		/* get the number of rows and columns */
beginGame:
	splash();

	char choice;
	do {
		choice = getch();
	} while (choice != ' ' && choice != 'h' && choice != 'q');

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

void print(int row, const char *format, ...) {
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));

	va_list  argptr;
	va_start(argptr, format);
	vsprintf(buffer, format, argptr);
	va_end(argptr);

	int col = (cols - strlen(buffer)) / 2;	
	row = rows / 2+ row;
	mvprintw(row, col, buffer);
}

void printBox(int width, int height) {
	int row = -height / 2 - 1;

	char *row_text = new char[width + 1];

	memset(row_text, '-', width);
	row_text[0] = '/';
	row_text[width - 1] = '\\';
	row_text[width] = 0;
	print(row++, row_text);

	memset(row_text, ' ', width);
	row_text[0] = row_text[width - 1] = '|';
	while(row < height / 2 + 1)
		print(row++, row_text);
	
	memset(row_text, '-', width);
	row_text[0] = '\\';
	row_text[width - 1] = '/';
	row_text[width] = 0;
	print(row++, row_text);

	delete row_text;
}

void splash() {
	clearScreen();

	printBox(48, 12);
	int row = -3;
	print(row++, "* WeeblyRPG *");
	row++;
	print(row++, "By Thomas Steinke");
	row++;
	print(row++, "(space) - Play");
	print(row++, "(h) - Help    ");
	print(row++, "(q) - Quit    ");

	refreshScreen();						/* Print it on to the real screen */
}

void helpScreen() {
	clearScreen();

	printBox(50, 20);

	int row = -8;
	print(row++, "WeeblyRPG is a text-based RPG where    ");
	print(row++, "everyone plays the same character!     ");
	row++;
	print(row++, "Work together to strengthen the hero,  ");
	print(row++, "and defeat the boss to add your name to");
	print(row++, "the trophy room!                       ");
	row++;
	print(row++, "Be careful, though:                    ");
	print(row++, "If someone beats the monster before    ");
	print(row++, "you, they get the fame and you get     ");
	print(row++, "...");
	print(row++, "NOTHING!!");
	row += 2;
	print(row++, "(space) - Go back");
	print(row++, "(q) - Quit       ");

	refreshScreen();						/* Print it on to the real screen */
}

std::string gameSelectScreen() {
	clearScreen();

	printBox(50, 11);
	print(-4, "* Enter Game Name *");
	print(-3, "Press (enter) to start");
	print(-2, "Leave blank for default Weebly game");
	int input_col = -18;

	refreshScreen();
	return input(0, input_col, ALLOW_EMPTY);
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
