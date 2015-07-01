#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <cstring>
#include <algorithm>

#include "ui.h"

using namespace std;

int UI::rows, UI::cols;
void *UI::screen;

int UI::init() {
	screen = initscr();
	noecho();
	
	// Get num rows and columns
	getmaxyx(stdscr, rows, cols);

	if (rows < ROWS || cols < COLS) {
		cleanup();
		cout << "Your terminal window is of size " << cols << "x" << rows << endl;
		cout << "Minimum size is " << COLS << "x" << ROWS << ". Please resize your window" << endl;

		return 1;
	}

	return 0;
}

void UI::cleanup() {
	endwin();
}

void UI::clear() { ::clear(); }
void UI::refresh() { ::refresh(); }
char UI::getchar() { return ::getch(); }
char UI::getchar(const vector<char> &options) {
	char ch;
	do {
		ch = ::getch();
	} while (std::find(options.begin(), options.end(), ch) == options.end());

	return ch;
}

std::string UI::input(int flags) {
	std::string output = "";
	char ch = getch();
	while (ch != '\n' || (output.size() == 0 && !(flags & ALLOW_EMPTY))) {
		if (ch == 127 && output.size() > 0) { // Backspace
			output = output.substr(0, output.size() - 1);

			int cury, curx;
			getyx((WINDOW *)screen, cury, curx);
			printw(" ");
			move(cury, curx - 1);
			printw(" ");
			move(cury, curx - 1);
		}
		else if (ch != 127 && ch != '\n') {
			output += ch;
			printw("%c", ch);
		}
	}

	return output;
}

Window::Window(int w, int h) { init(w, h, 0, 0); }
Window::Window(int w, int h, int x, int y) {	init(w, h, x, y); }

Window::Window(string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		printError(filename + " not found.");
		return;
	}

	int w, h;
	file >> w >> h;
	init(w, h, 0, 0);

	string message;
	while (getline(file, message)) {
		printlncenter(message.c_str());
	}
}

void Window::init(int w, int h, int x, int y) {
	x1 = x + UI::getCols() / 2 - w / 2;
	y1 = y + UI::getRows() / 2 - h / 2;
	x2 = x1 + w;
	y2 = y1 + h;

	curx = x1 + PADDING_X;
	cury = y1 + PADDING_Y;

	int row = y1;

	// Print the box
	char row_text[128];
	memset(row_text,  '-', w);
	row_text[0]     = '/';
	row_text[w - 1] = '\\';
	row_text[w]     = 0;
	mvprintw(row++, x1, row_text);

	row_text[0]         = '\\';
	row_text[w - 1] = '/';
	mvprintw(y2, x1, row_text);

	memset(row_text, ' ', w);
	row_text[0] = row_text[w - 1] = '|';
	while (row < y2)
		mvprintw(row++, x1, row_text);
}

void Window::printMessage(std::string title, std::string message) {
	int len = message.size();
	if (title.size() > message.size()) {
		len = title.size();
	}

	Window messageWindow(len + 4, 6, 0, 0);

	messageWindow.printlncenter(title.c_str());
	messageWindow.println();
	messageWindow.printlncenter(message.c_str());
	messageWindow.printlncenter("Press any key to continue...");
}

void Window::println(int lines) {
	curx = x1 + PADDING_X;
	cury += lines;

	move(cury, curx);
}

void Window::moveCursor(int dx, int dy) {
	cury += dy;
	curx += dx;

	move(cury, curx);
}

void Window::setCursor(int rx, int ry) {
	if (rx >= 0)
		curx = x1 + rx;
	else
		curx = x2 + rx + 1;

	if (ry >= 0)
		cury = y1 + ry;
	else
		cury = y2 + ry + 1;

	move(cury, curx);
}

void Window::println(Align alignment, const char *format, va_list args) {
	print(alignment, format, args);

	println();
}

void Window::print(Align align, const char *format, va_list args) {
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
		
	vsprintf(buffer, format, args);
	
	int len = strlen(buffer);
	
	int col = curx;
	if (align == Center) { 
		col = (x1 + x2 - len) / 2;
	}
	else if (align == Right) {
		col = x2 - PADDING_X - len;
	}
	
	mvprintw(cury, col, buffer);
	curx += len;
}

#define PRINT(alignment, function) va_list args;\
	va_start(args, format);\
	function(alignment, format, args);\
	va_end  (args);

void Window::print(const char *format, ...) { PRINT(Left, print); }
void Window::printlncenter(const char *format, ...) { PRINT(Center, println); }
void Window::printlnleft(const char *format, ...) { PRINT(Left, println); }
void Window::printlnright(const char *format, ...) { PRINT(Right, println); }
