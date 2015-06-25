#include <ncurses.h>
#include <cstring>

#include "box.h"
#include "main.h"

ScrBox::ScrBox(int w, int h, Point c) {
	if (w == 0) return;

	c.y += getRows() / 2;
	c.x += getCols() / 2;

	int row = c.y - h / 2;
	int last_row = row + h;
	int col = c.x - w / 2;

	top_left.x = col + PADDING_X;
	top_left.y = row + PADDING_Y;

	cursor.x = top_left.x + PADDING_X;
	cursor.y = top_left.y;
	
	bottom_right.x = col + w - PADDING_X;
	bottom_right.y = row + h - PADDING_Y;

	// Print the box
	char row_text[128];
	memset(row_text,  '-', w);
	row_text[0]     = '/';
	row_text[w - 1] = '\\';
	row_text[w]     = 0;
	mvprintw(row++, col, row_text);

	row_text[0]         = '\\';
	row_text[w - 1] = '/';
	mvprintw(last_row, col, row_text);

	memset(row_text, ' ', w);
	row_text[0] = row_text[w - 1] = '|';
	while (row < last_row)
		mvprintw(row++, col, row_text);
}

void ScrBox::println() {
	cursor.x = top_left.x + PADDING_X;
	cursor.y ++;

	move(cursor.y, cursor.x);
}

void ScrBox::moveCursor(int dx, int dy) {
	cursor.y += dy;
	cursor.x += dx;

	move(cursor.y, cursor.x);
}

void ScrBox::setCursor(int rx, int ry) {
	if (rx >= 0)
		cursor.x = top_left.x + rx;
	else
		cursor.x = bottom_right.x + rx + 1;

	if (ry >= 0)
		cursor.y = top_left.y + ry;
	else
		cursor.y = bottom_right.y + ry + 1;

	move(cursor.y, cursor.x);
}

void ScrBox::println(Align alignment, const char *format, va_list args) {
	print(alignment, format, args);

	println();
}

void ScrBox::print(Align align, const char *format, va_list args) {
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
		
	vsprintf(buffer, format, args);
	
	int len = strlen(buffer);
	
	int col = cursor.x;
	if (align == Center) col = (bottom_right.x + top_left.x - len) / 2;
	if (align == Right) col = (bottom_right.x - PADDING_X) - len;
	
	mvprintw(cursor.y, col, buffer);
	cursor.x += len;
}

#define PRINT(alignment, function) va_list args;\
	va_start(args, format);\
	function(alignment, format, args);\
	va_end  (args);

void ScrBox::print(const char *format, ...) { PRINT(Left, print); }
void ScrBox::printlncenter(const char *format, ...) { PRINT(Center, println); }
void ScrBox::printlnleft(const char *format, ...) { PRINT(Left, println); }
void ScrBox::printlnright(const char *format, ...) { PRINT(Right, println); }

std::string _input(int y, int x, int f) { return input(y, x, f); }

std::string ScrBox::input(int flags) {
	return _input(cursor.y - getRows() / 2, cursor.x - getCols() / 2, flags);
}

char ScrBox::getchar() { return getch(); }
