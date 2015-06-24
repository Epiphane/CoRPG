#include <iostream>
#include <sstream>

#include "main.h"
#include "gameinfo.h"

using namespace std;

/* Game file encryption */
string encrypt(string message) {
	for (int i = 0; i < message.size(); i ++)
		message[i] = message[i];

	return message;
}

string decrypt(string message) {
	return encrypt(message) + "\n";
}

void write_file(fstream &file, stringstream &input) {
	string message;
	while (!input.eof()) {
		getline(input, message);
		file << decrypt(message);
	}
}

void read_file(fstream &file, stringstream &output) {
	string message;
	while (!file.eof()) {
		getline(file, message);
		output << decrypt(message);
	}
}

void GameObject::load(stringstream &input) {
	input >> name;
	input >> level;
	input >> health;

	string line;
	input >> line;
	while (line != ";" && !input.eof()) {
		string val;
		getline(input, val);

		properties[line] = val.substr(1);

		input >> line;
	}
}

void GameObject::save(stringstream &output) {
	output << name << "\n" << level << "\n" << health << "\n";

	unordered_map<string, string>::iterator it = properties.begin();
	while (it != properties.end()) {
		output << it->first << " " << it->second << "\n";
		it ++;
	}

	output << ";" << "\n";
}

std::string GameObject::getProperty(std::string prop) {
	return properties[prop];
}

void GameObject::setProperty(std::string prop, std::string val) {
	properties[prop] = val;
}

void GameObject::infoPage(string title) {
	clearScreen();

	ScrBox page(30, 11 + properties.size());

	page.printlncenter(title.c_str());
	page.println();
	page.println();

	page.printlncenter("Name: %s", name.c_str());
	page.printlncenter("Health: %d", health);

	unordered_map<string, string>::iterator it = properties.begin();
	while (it != properties.end()) {
		page.printlncenter("%s: %s", it->first.c_str(), it->second.c_str());
		it ++;
	}

	page.println();
	page.println();
	page.printlncenter("[Enter] - Continue");

	refreshScreen();

	char ch = 0;
	while (ch != 27 && ch != '\n') {
		ch = page.getchar();
	}
}
