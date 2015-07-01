#include <iostream>
#include <curl/curl.h>

#include "ui.h"
#include "game.h"

using namespace std;

int main() {
	if (UI::init() != 0) {
		return 1;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	char choice = 0;
	while (choice != 'q') {
		UI::clear();
		Window("splash.txt");
		choice = UI::getchar(vector<char>({ ' ', 'h', 'q'}));
		
		if (choice == ' ') {
			// Play game!
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
