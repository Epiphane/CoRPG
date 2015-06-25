#include <string>
#include <string.h>
#include <iostream>
#include <ncurses.h>
#include <sstream>

#include "game.h"
#include "lua_shell.h"

using namespace std;

ScrBox Lua_window(0, 0, CENTER);
int Lua_openWindow(lua_State *L) {
	int x = lua_tonumber(L, 1);
	int y = lua_tonumber(L, 2);
	int w = lua_tonumber(L, 3);
	int h = lua_tonumber(L, 4);
	
	Point c = {w, h};
	Lua_window = ScrBox(x, y, c);
	return 0;
}

int Lua_print(lua_State *L) {
	int n = lua_gettop(L);
	if (n == 0) return 0;

	const char *_message = lua_tostring(L, 1);
	char message[256];
	memcpy(message, _message, strlen(_message));

	Lua_window.print(_message);

	return 0;
}

int Lua_println(lua_State *L) {
	Lua_print(L);
	Lua_window.println();
	
	return 0;
}	

int Lua_setcursor(lua_State *L) {
	int rx = lua_tointeger(L, 1);
	int ry = lua_tointeger(L, 2);

	Lua_window.setCursor(rx, ry);

	return 0;
}

LuaScript::LuaScript(const std::string& filename) : isComplete(false) {
	L = luaL_newstate();
	if (filename != "") // Must be initialized later!
		init(filename);
}
LuaScript::~LuaScript() {
	if (L) lua_close(L);
}

void LuaScript::init(const std::string& filename) {
	if (luaL_loadfile(L, filename.c_str())) {
		L = NULL;
		printError("Script not loaded: " + filename);
		isComplete = true;
		cout << "wat" << endl;
		return;
	}
	else
		luaL_openlibs(L);
}

#define DEF_FUN(name, fundef) lua_pushcfunction(L, fundef);\
	lua_setglobal(L, name);
#define SAFE_PCALL(call, methodName) if (call) {\
	printError("Script execution failed [" + string(methodName) + "]: " + string(lua_tostring(L, -1)));\
	L = NULL;\
	return;\
	}
void LuaScript::run() {
	if (!L) {
		printError("Script not loaded");
		isComplete = true;
		return;
	}
	
	DEF_FUN("window", Lua_openWindow);
	DEF_FUN("print", Lua_print);
	DEF_FUN("println", Lua_println);
	DEF_FUN("cursor", Lua_setcursor);
	pre_run();

	SAFE_PCALL(lua_pcall(L, 0, 0, 0), "main");

	post_run();
}

// GameObject functionality
Region *running_region;
int GO_create(lua_State *L) {
	GameObject **box = (GameObject **)lua_newuserdata(L, sizeof(GameObject *));
	luaL_setmetatable(L, "GameObject");

	string name = string(lua_tostring(L, -1));
	*box = running_region->game->newObject(name);

	return 1;
}
int GO_get(lua_State *L) {
	string name = string(lua_tostring(L, -1));

	GameObject **box = (GameObject **)lua_newuserdata(L, sizeof(GameObject *));
	luaL_setmetatable(L, "GameObject");

	*box = running_region->game->getObject(name);

	return 1;
}
int GO_setProperty(lua_State *L) {
	GameObject **box = (GameObject **)luaL_checkudata(L, 1, "GameObject");
	string key = lua_tostring(L, 2);
	string value = lua_tostring(L, 3);

	if (key == "name")
		(*box)->name = value;
	else if (key == "health")
		(*box)->health = atoi(value.c_str());
	else if (key == "maxhealth")
		(*box)->maxhealth = atoi(value.c_str());
	else if (key == "level")
		(*box)->level = atoi(value.c_str());
	else
		(*box)->setProperty(key, value);

	return 0;
}
int GO_getProperty(lua_State *L) {
	GameObject **box = (GameObject **)luaL_checkudata(L, 1, "GameObject");
	string key = lua_tostring(L, 2);

	string prop = "";
	if (key == "name")
		prop = (*box)->name;
	else if (key == "health")
		prop = std::to_string((long long int) (*box)->health);
	else if (key == "maxhealth")
		prop = std::to_string((long long int) (*box)->maxhealth);
	else if (key == "level")
		prop = std::to_string((long long int) (*box)->level);
	else
		prop = (*box)->getProperty(key).c_str();
		
	lua_pushstring(L, prop.c_str());

	return 1;
}
int GO_viewInfo(lua_State *L) {
	GameObject **box = (GameObject **)luaL_checkudata(L, 1, "GameObject");
	(*box)->infoPage((*box)->name);

	return 0;
}

static const struct luaL_Reg Lua_gameobject_funcs[] = {
	{"new", GO_create},
	{"get", GO_getProperty},
	{"set", GO_setProperty},
	{"info", GO_viewInfo},
	{NULL, NULL}
};

int Region_depend(lua_State *L) {
	string dependency = lua_tostring(L, -1);

	running_region->game->addDependency(dependency);

	return 0;
}

// Region-specific stuff
Region::Region(const std::string& name, Game *g) : LuaScript(""), game(g) {
	init("world/" + name + ".lua");
	if (error()) return;

	// Create GameObject metatable
	luaL_newmetatable(L, "GameObject");
	luaL_setfuncs(L, Lua_gameobject_funcs, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");

	lua_setglobal(L, "GameObject");
}

int Lua_move(lua_State *L) {
	running_region->move(string(lua_tostring(L, 1)));
	return 0;
}

void Region::move(const string &next) {
	game->setRegion(next);
	isComplete = true;
}

fstream region_file;
int Lua_writetofile(lua_State *L) {
	region_file << lua_tostring(L, 1) << endl;

	return 0;
}

void Region::pre_run() {
	DEF_FUN("move", Lua_move);
	DEF_FUN("depend", Region_depend);
	DEF_FUN("findObject", GO_get);
	DEF_FUN("writeln", Lua_writetofile);

	GameObject **obj = (GameObject **)lua_newuserdata(L, sizeof(GameObject *));
	luaL_setmetatable(L, "GameObject");
	*obj = game->getPlayer();

	lua_setglobal(L, "player");

	running_region = this;
}

#define GET_AND_CHECK(name) lua_getglobal(L, name);\
	if (lua_isnil(L, -1)) {\
		isComplete = true;\
		printError(string(name) + " function is not defined");\
		return;\
	}
void Region::render() {
	// be careful with this function bro. Lots of things could go wrong
	GET_AND_CHECK("render");

	clear();
	mvprintw(0, 0, ".-----------------------.");
	mvprintw(1, 0, "| Press ESC to pause... |");
	mvprintw(2, 0, "^-----------------------^");
	SAFE_PCALL(lua_pcall(L, 0, 0, 0), "render");
	refresh();			  
}

void Region::post_run() {	
	// Load current state info
	string save_file = "save/._" + game->getName() + "_regioninfo_" + game->getRegion();
	lua_getglobal(L, "loadData");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
	}
	else {
		region_file.open(save_file, iostream::in);
		if (region_file.is_open()) {
			lua_newtable(L);

			int line = 1;
			string message;
			while (!region_file.eof()) {
				getline(region_file, message);

				if (message.size() == 0) continue;

				lua_pushnumber(L, line);
				lua_pushstring(L, message.c_str());
				lua_settable(L, -3);
	
				line ++;
			}
			
			SAFE_PCALL(lua_pcall(L, 1, 0, 0), "load");
		}
	}
	region_file.close();

	// Get render and update functions
	GET_AND_CHECK("render");
	GET_AND_CHECK("update");

	char ch = 0;
	while (!isComplete) {
		clear();
		mvprintw(0, 0, ".-----------------------.");
		mvprintw(1, 0, "| Press ESC to pause... |");
		mvprintw(2, 0, "^-----------------------^");
		lua_pushvalue(L, -2); // Queue up render()
		SAFE_PCALL(lua_pcall(L, 0, 0, 0), "render");
		refresh();			  

		ch = getch();
		if (ch == 27 && !game->pause())
			isComplete = true;
		else {
			if (ch == 127) ch = '\b';

			lua_pushvalue(L, -1); // Queue up update()
			lua_pushlstring(L, &ch, 1);
			SAFE_PCALL(lua_pcall(L, 1, 0, 0), "update");
		}
	}

	// Save current state
	lua_getglobal(L, "saveData");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
	}
	else {
		region_file.open(save_file, iostream::out);

		if (region_file.is_open()) {
			SAFE_PCALL(lua_pcall(L, 0, 0, 0), "save");
		}
	}
	region_file.close();
}

// Error reporting
void LuaScript::printError(const string &message) {
	ScrBox error(4 + message.size(), 7);
	error.printlncenter("Error"); 
	error.printlncenter(message.c_str());
	error.println();
	error.printlncenter("Press any button to continue...");
	
	refresh();
	
	getch();
}

void LuaScript::printError(const string &variable, const string &reason) {
	printError("Can't get [" + variable + "]: " + reason);
}

// Default values
template<> inline bool LuaScript::lua_get(const std::string& variableName) {
	return (bool)lua_toboolean(L, -1);
}
template<> inline float LuaScript::lua_get(const std::string& variableName) {
	if(!lua_isnumber(L, -1)) {
		printError(variableName, "Not a number");
	}
	return (float)lua_tonumber(L, -1);
}
template<> inline int LuaScript::lua_get(const std::string& variableName) {
	if(!lua_isnumber(L, -1)) {
		printError(variableName, "Not a number");
	}
	return (int)lua_tonumber(L, -1);
}
template<> inline std::string LuaScript::lua_get(const std::string& variableName) {
	if(lua_isstring(L, -1))
		return string(lua_tostring(L, -1));
	else
		return "null";
}

// Value retrieval
bool LuaScript::push_var(const string &var) {
	if (level == 0)
		lua_getglobal(L, var.c_str());
	else
		lua_getfield(L, -1, var.c_str());
	
	return !lua_isnil(L, -1);
}

template<typename T>
T LuaScript::get(const string& variable) {
	if (!L) {
		printError(variable, "Script not loaded");
		return lua_getdefault<T>();
	}

	level = 0;
	string var = "";
	for (unsigned int i = 0; i < variable.size(); i ++) {
		if (variable.at(i) == '.') {
			if (!push_var(var)) {
				printError(variable, var + " is not defined");
				return lua_getdefault<T>();
			}
			else {
				var = "";
				level ++;
			}
		}
		else var += variable.at(i);
	}
	if (!push_var(var)) {
		printError(variable, var + " is not defined");
		return lua_getdefault<T>();
	}

	T result = lua_get<T>(variable);
	lua_pop(L, level + 1);
	return result;
}

// Cleaning the Lua Stack
void LuaScript::clean() {
	lua_settop(L, 0);
}
