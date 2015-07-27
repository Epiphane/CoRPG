#ifndef GAME_INFO
#define GAME_INFO

#include <string>
#include <unordered_map>
#include <fstream>

#include "json/json.h"

/* File saving */
class GameObject {
public:
	Json::Value state;
	std::string name, region;
	bool _new;

	GameObject(Json::Value info) : _new(false) { init(info); }
	GameObject(std::string name, std::string region) : name(name), region(region), _new(false) {};
	void init(Json::Value info);
	
	void fetch();
	void save();
	Json::Value get(std::string val);
	void update(Json::Value properties);

	void act(GameObject *other, Json::Value action);
	void damage(GameObject *other, int damage);

	virtual void infoPage(std::string title);
};

#endif
