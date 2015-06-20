#include <string>
#include "box.h"

// For text, pretend the center of the screen is 0,0
void clearScreen();
void refreshScreen();

int getRows();
int getCols();

#define NONE 0
#define ALLOW_EMPTY 1
std::string input(int cursor_row, int cursor_col, int flags = NONE);
