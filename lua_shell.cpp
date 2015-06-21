#include <string>
#include <iostream>
#include <ncurses.h>
#include <sstream>

#include "game.h"
#include "lua_shell.h"

using namespace std;

ScrBox *L_window = NULL;

void L_openWindow(int w, int h, int cx, int cy) {
	if (L_window != NULL)
		delete L_window;

	Point center = { cx, cy };
	L_window = new ScrBox(w, h, center);
}

void L_print(const string message) {
	stringstream input(message);
	for (string line;; ) {
		L_window->print(line.c_str());
		
		if (getline(input, line))
			L_window->println();
		else
			break;
	}
}

void L_println(const string message) {
	L_print(message);
	L_window->println();
}

char L_getch() { return getch(); }
string L_input(int flags = 0) { return L_window->input(flags); }

bool stateChanged;
void L_setState(std::string state) {
	getCurrentGame()->setRegion(state);

	stateChanged = true;
}

using namespace luabridge;

void run_script(string filename) {
	stateChanged = false;

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	Namespace global = getGlobalNamespace(L);
	global.addFunction("window", L_openWindow);
	global.addFunction("print", L_print);
	global.addFunction("println", L_println);
	global.addFunction("getch", L_println);
	global.addFunction("input", L_input);
	global.addFunction("move", L_setState);
	
	int err = luaL_loadfile(L, filename.c_str());
	if (err == 0) {
		try {
			lua_pcall(L, 0, 0, 0);

			LuaRef render = getGlobal(L, "render");
			LuaRef update = getGlobal(L, "update");

			LuaRef s = getGlobal(L, "testString");

			while (!stateChanged && !update.isNil()) {
				clear();
				if (!render.isNil())
					render();
				refresh();
				
				char ch = getch();
				if (ch != 27) { // Escape
					update(ch);
				}
				else 
					if (!getCurrentGame()->pause()) // 0 means stop level
						break;
			}
		}
		catch (LuaException e) {
			int len = strlen(e.what());
			if (len < 31) len = 32;

			ScrBox error(len + 4, 7);
			error.printlncenter("Error", filename.c_str()); 
			error.printlncenter("%s", e.what());
			error.println();
			error.printlncenter("Press any button to continue...");

			refresh();

			getch();
		}
	}
	else {
		ScrBox error(40, 5);
		error.printlncenter("Error loading %s: %d", filename.c_str(), err);

		refresh();

		getch();
	}
}
