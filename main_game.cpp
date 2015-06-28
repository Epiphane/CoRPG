/* Screen */
#include <ncurses.h>
/* Output */
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* Errors */
#include <errno.h>
#include <string.h>
/* Changing directories */
#include <dirent.h>
#include <unistd.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include "client_server.h"
#include "main.h"
#include "box.h"

#define SUCCESS 0
#define FAIL 1

using namespace std;

int chdir_to_executable();

int main(int argc, char *argv[]) {
	if (chdir_to_executable() != SUCCESS) {
		return 1;
	}
	
	// Run server if it hasn't been opened already
	if (!fork()) {
		int server_logs[2];
		const int flags = O_WRONLY | O_APPEND | O_CREAT;
		const int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
		server_logs[0] = open(".server.cout", flags, mode);
		server_logs[1] = open(".server.cerr", flags, mode);
		if (server_logs[0] < 0 || server_logs[1] < 0) {
			cerr << "Error opening server logs: " << strerror(errno) << endl;
		}
		else {
			// Redirect output
			dup2(server_logs[0], 1);
			dup2(server_logs[1], 2);
			// Avoid command line input
			close(0);
			
			// Create child process with server
			execl("funserver", "funserver", NULL);
			cout << "Error opening server: " << strerror(errno) << endl;
		}

		return 1;
	}

	// Client game now!
	cout << "Opening connection to server..." << endl;
	pid_t server = get_server();
	if (server == -1) {
		return 1;
	}
	cout << "Server: " << server << endl;
	kill(server, REQCONNECT);

	return 0;
}

int chdir_to_executable() {
#ifdef __APPLE__	
	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) != 0) {
		cerr << "buffer too small; need size " << size << endl;
		return FAIL;
	}
#else
	char path[1024];
	readlink("/proc/self/exe", path, 1024);
#endif

	// Change to directory of executable
	path[string(path).find_last_of('/')] = 0;
	if(chdir(path)) {
		cerr << "Error changing to " << path << ": " << strerror(errno) << endl;
		return FAIL;
	}

	return SUCCESS;
}

void splash();
void helpScreen();
std::string gameSelectScreen();

int rows, cols;
WINDOW *scr;

int getRows() { return rows; }
int getCols() { return cols; }

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
