#include <iostream>
#include <sstream>

#include "curl.h"
#include "gameinfo.h"
#include "ui.h"

using namespace std;

void GameObject::init(Json::Value info) {
	name = info["name"].asString();
	region = info["region"].asString();
	
	properties = info["properties"];
	
	level = health = maxhealth = 0;
	
	if (properties.isMember("level"))
		level     = properties["level"].asInt();
	if (properties.isMember("health"))
		health    = properties["health"].asInt();
	if (properties.isMember("max_health"))
		maxhealth = properties["max_health"].asInt();
}

void GameObject::fetch() {
	Json::Value info = Curl::GET(region + "/" + name);
	
	init(info);
}

void GameObject::save() {

}

Json::Value GameObject::get(std::string prop) {
	if (prop == "name")
		return name;
	else if (prop == "region")
		return region;

	return properties[prop];
}

void GameObject::set(std::string prop, Json::Value val) {
	if (prop == "health")
		return;
	else if (prop == "region")
		return;

	properties[prop] = val;
}

void GameObject::infoPage(string title) {
	UI::clear();

	Window page(30, 11 + properties.size());

	page.printlncenter(title.c_str());
	page.println();
	page.println();

	page.printlnleft("Name: %s", name.c_str());

	Json::Value::Members keys = properties.getMemberNames();
	Json::Value::Members::iterator it = keys.begin();
	while (it != keys.end()) {
		Json::Value value    = properties[*it];
		Json::ValueType type = value.type();
		if (type == Json::intValue)
			page.printlnleft("%s: %s", it->c_str(), std::to_string(properties[*it].asInt()).c_str());
		else
			page.printlnleft("%s: %s", it->c_str(), properties[*it].asCString());

		it ++;
	}

	page.println();
	page.println();
	page.printlncenter("[Enter] - Continue");

	UI::refresh();

	UI::getchar(vector<char>({ 27, '\n' }));
}
