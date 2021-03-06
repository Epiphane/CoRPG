#include <iostream>
#include <curl/curl.h>
#include <string.h>
#include <signal.h>
/* Changing directories */
#include <dirent.h>
#include <unistd.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#include <string>

#include "ui.h"
#include "game.h"

#define SUCCESS 0
#define FAIL 1

using namespace std;

int chdir_to_executable();

bool game_playing = false;
void handle_signal(int s) {
	if (game_playing) {
		Window::printMessage("Signal " + to_string((long long int)s), "Game process interrupted");
		Game::instance()->save();
		UI::getchar();
	}

	UI::cleanup();

	exit(1);
}

int main() {
	if (chdir_to_executable() != SUCCESS) {
		return 1;
	}

	if (UI::init() != 0) {
		return 1;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	signal(SIGINT, handle_signal);

	char choice = 0;
	while (choice != 'q') {
		UI::clear();
		Window("splash.txt");
		choice = UI::getchar(vector<char>({ ' ', 'h', 'q'}));
		
		if (choice == ' ') {
			// Play game!
			game_playing = true;
			Game::instance()->play();

			break;
		}
		else if (choice == 'h') {
			Window("help.txt");
			choice = UI::getchar(vector<char>({ ' ', 'q' }));
		}
	}

	UI::cleanup();

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
