#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>

#include "main.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

/* Script class */ 
class LuaScript {
public:
	LuaScript(const std::string& filename);
	~LuaScript();

	virtual void run();
	void printError(const std::string& message);
	void printError(const std::string& variableName, const std::string& reason);
 
	template<typename T> T get(const std::string& variableName);
 
	// Generic get
	template<typename T>
	T lua_get(const std::string& variableName) {
		return 0;
	}
 
	// Generic default get
	template<typename T>
	T lua_getdefault() {
		return 0;
	}
protected:
	lua_State* L;

	int level;
	bool push_var(const std::string &var);
	void clean();

	virtual void pre_run() {};
	virtual void post_run() {};
};
	
template<>
inline std::string LuaScript::lua_getdefault() { return "null"; }

class Game;
class Region : public LuaScript {
public:
	Region(std::string name, Game *g) : LuaScript("world/" + name + ".lua"), game(g), isComplete(false) {};

	void move(const std::string &region);
private:
	Game *game;
	bool isComplete;

	void pre_run();
	void post_run();
};

#endif
