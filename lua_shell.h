#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>

#include "main.h"

#define LUAW_NO_EXTERN_C
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include "luawrapper.hpp"

/* Script class */ 
class LuaScript {
public:
	LuaScript(const std::string& filename);
	void init(const std::string& filename);
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
	Region(const std::string& name, Game *g);
	void move(const std::string &region);
	bool error() { return L == NULL; }
private:
	Game *game;
	bool isComplete;

	void pre_run();
	void post_run();
};

#endif
