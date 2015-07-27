#include <iostream>
#include <sstream>
#include <string>

#include "curl.h"
#include "gameinfo.h"
#include "ui.h"

using namespace std;

void GameObject::init(Json::Value info) {
	name = info["name"].asString();
	region = info["region"].asString();

	state = info;
}

void GameObject::fetch() {
	Json::Value info = Curl::GET(region + "/" + name);
	
	init(info);
}

void GameObject::save() {
	Json::Value result = Curl::PUT(name, region, state);
	
	init(result);
}

Json::Value GameObject::get(std::string prop) {
	if (prop == "name" && !state["properties"]["nickname"].isNull())
		return state["properties"]["nickname"];
	if (prop == "name" || prop == "region")
		return state[prop];
	if (prop == "new")
		return Json::Value(_new ? "true" : "");

	return state["properties"][prop];
}

void GameObject::act(GameObject *other, Json::Value action) {
	Json::Value data;
	Json::Value vSelf, vOther;
	vSelf["name"] = name;
	vSelf["region"] = region;
	data["self"] = vSelf;
	data["action"] = action;
	if (other) {
		vOther["name"] = other->name;
		vOther["region"] = other->region;	

		data["other"] = vOther;
	}

	Json::Value result = Curl::ACT(data);

	if (result.isMember("error")) {
		Window::printMessage("Server error", result["error"].toStyledString().c_str());
		UI::getchar();
	}
	else {
		init(result["self"]);

		if (other)
			other->init(result["other"]);
	}
}

void GameObject::update(Json::Value properties) {
	properties["method"] = "set";

	act(NULL, properties);
}

void GameObject::damage(GameObject *other, int amt) {
	Json::Value action;

	action["method"] = "attack";
	action["damage"] = amt;

	act(other, action);
}

void GameObject::infoPage(string title) {
	UI::clear();

	Window page(30, 11 + state["properties"].size());

	page.printlncenter(title.c_str());
	page.println();
	page.println();

	page.printlnleft("Name: %s", get("name").asCString());
	page.printlnleft("Class: %s", get("class").asCString());
	page.printlnleft("Level: %d", get("level").asInt());
	page.printlnleft("Health: %d / %d", get("health").asInt(), get("max_health").asInt());

	Json::Value::Members keys = state["properties"].getMemberNames();
	Json::Value::Members::iterator it = keys.begin();
	while (it != keys.end()) {
		Json::Value value    = get(*it);
		Json::ValueType type = value.type();
		if (type == Json::intValue) {
			page.printlnleft("%s: %d", it->c_str(), value.asInt());
		}
		else if (type == Json::stringValue) {
			page.printlnleft("%s: %s", it->c_str(), value.asCString());
		}

		it ++;
	}

	page.println();
	page.println();
	page.printlncenter("[Enter] - Continue");

	UI::refresh();

	UI::getchar(vector<char>({ 27, '\n' }));
}
