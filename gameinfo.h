#ifndef GAME_INFO
#define GAME_INFO

#include <string>
#include <unordered_map>
#include <fstream>

#define GAME_FILE "save/._gameinfo"

/* File saving */
std::string encrypt(std::string message);
std::string decrypt(std::string message);
void read_file(std::fstream &file, std::stringstream &output);
void write_file(std::fstream &file, std::stringstream &input);

class GameObject {
private:
	std::unordered_map<std::string, std::string> properties;

	std::string id;

public:
	std::string name;
	int health, maxhealth, level;

	GameObject(std::string id) : id(id), level(1) { properties.clear(); };

	std::string getID() { return id; }
	std::string getProperty(std::string prop);
	void setProperty(std::string prop, std::string val);

	virtual void load(std::stringstream &input);
	virtual void save(std::stringstream &input);
	virtual void infoPage(std::string title);
};
#endif
