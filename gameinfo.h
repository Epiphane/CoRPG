#ifndef GAME_INFO
#define GAME_INFO

#include <string>
#include <unordered_map>
#include <fstream>

#define GAME_FILE "._gameinfo"

/* File saving */
std::string encrypt(std::string message);
std::string decrypt(std::string message);
void read_file(std::fstream &file, std::stringstream &output);
void write_file(std::fstream &file, std::stringstream &input);

class GameObject {
private:
	std::unordered_map<std::string, std::string> properties;

public:
	std::string name;
	int health, level;

	GameObject() : level(1) { properties.clear(); };

	std::string getProperty(std::string prop);
	void setProperty(std::string prop, std::string val);

	virtual void load(std::stringstream &input);
	virtual void save(std::stringstream &input);
	virtual void infoPage(std::string title);
};
#endif
