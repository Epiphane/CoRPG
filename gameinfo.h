#ifndef GAME_INFO
#define GAME_INFO

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "json/json.h"

/* File saving */
class GameObject {
public:
	std::vector<GameObject *> possessions;
	Json::Value state;
	std::string name, region;
	bool _new, possessions_fetched;

	GameObject(Json::Value info) : _new(false), possessions_fetched(false) { init(info); possessions.clear(); }
	GameObject(std::string name, std::string region) : name(name), region(region), _new(false), possessions_fetched(false) { possessions.clear(); };
	void init(Json::Value info);
	
	void fetch();
	void save();
	Json::Value get(std::string val);
	std::vector<GameObject *> getPossessions();
	void update(Json::Value properties);

	void act(GameObject *other, Json::Value action);
	void damage(GameObject *other, int damage);

	virtual void infoPage(std::string title);
};

#endif
