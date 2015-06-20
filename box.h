#ifndef BOX
#define BOX

#include <string>

typedef struct { int x, y; } Point;
const Point CENTER = { 0, 0 };
typedef enum { Left, Center, Right } Align;

class ScrBox {
private:
	const static int PADDING_X = 2;
	const static int PADDING_Y = 2;

	Point top_left, bottom_right;
	Point cursor;

public:
	ScrBox(int width, int height, Point center = CENTER);
	
	void moveCursor(int dx, int dy) { cursor.x += dx; cursor.y += dy; }
	void println(Align align, const char *format, va_list args);
	void print(Align align, const char *format, va_list args);

	void println();
	void print(const char *format, ...);
	void printlnleft(const char *format, ...);
	void printlncenter(const char *format, ...);
	void printlnright(const char *format, ...);

	std::string input(int flags = 0);
};

#endif
