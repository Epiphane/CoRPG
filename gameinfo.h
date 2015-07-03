#ifndef GAME_INFO
#define GAME_INFO

#include <string>
#include <unordered_map>
#include <fstream>

#include "json/json.h"

/* File saving */
class GameObject {
private:
	Json::Value properties;

public:
	std::string name, region;
	int health, maxhealth, level;

	GameObject(std::string name, std::string region) : name(name), region(region) { properties.clear(); };
	
	void sync() { save(); fetch(); }
	void fetch();
	void save();
	Json::Value get(std::string val);
	void set(std::string prop, Json::Value val);

	virtual void infoPage(std::string title);
};

#endif
