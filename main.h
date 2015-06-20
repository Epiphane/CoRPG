#include <string>

// For text, pretend the center of the screen is 0,0
void clearScreen();
void refreshScreen();
void printBox(int width, int height);
void print(int row, const char *format, ...);

#define NONE 0
#define ALLOW_EMPTY 1
#define ALIGN_RIGHT 2
std::string input(int cursor_row, int cursor_col, int flags = NONE);
