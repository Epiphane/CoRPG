#ifndef GAME_INFO
#define GAME_INFO

#include <string>
#include <unordered_map>
#include <fstream>

/* File saving */
class GameObject {
private:
	std::unordered_map<std::string, std::string> properties;

public:
	std::string name;
	int health, maxhealth, level;

	GameObject() : level(1) { properties.clear(); };

	std::string getProperty(std::string prop);
	void setProperty(std::string prop, std::string val);

	virtual void load(std::stringstream &input);
	virtual void save(std::stringstream &input);
	virtual void infoPage(std::string title);
};
#endif
