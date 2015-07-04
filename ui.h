#ifndef UI_H
#define UI_H 

#include <vector>
#include <string>

const int ALLOW_EMPTY = 1;

class UI {
private:
	static void *screen;
	static int rows, cols;

public:
	static const int ROWS = 30;
	static const int COLS = 95;

	static int init();
	static void cleanup();
	static void clear();
	static void refresh();
	
	static int getRows() { return rows; }
	static int getCols() { return cols; }

	static char getchar();
	static char getchar(const std::vector<char> &options);
	static std::string input(int flags = 0);
};

typedef enum { Left, Center, Right } Align;
class Window {
private:
	const static int PADDING_X = 2;
	const static int PADDING_Y = 2;

	int x1, x2, y1, y2;
	int curx, cury;

public:
	Window(int w, int h);
	Window(int w, int h, int x, int y);
	Window(std::string filename);
	void init(int w, int h, int x, int y);

	static void printMessage(std::string title, std::string message);
	static void printError(std::string message) { printMessage("Error", message); }
	
	void moveCursor(int dx, int dy);
	void setCursor(int rx, int ry);
	void println(Align align, const char *format, va_list args);
	void print(Align align, const char *format, va_list args);

	void println(int lines = 1);
	void print(const char *format, ...);
	void printlnleft(const char *format, ...);
	void printlncenter(const char *format, ...);
	void printlnright(const char *format, ...);
};

#endif /* UI_H */
