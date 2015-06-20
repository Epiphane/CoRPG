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
	input >> health;

	string line;
	input >> line;
	while (line != ";" && !input.eof()) {
		cout << line << endl;
		string val;
		getline(input, val);

		properties[line] = val;

		input >> line;
	}
}

void GameObject::save(stringstream &output) {
	output << name << "\n" << health << "\n";

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

	printBox(30, 11 + properties.size());
	int row = -properties.size() / 2 - 3;

	print(row, title.c_str());
	row += 2;

	print(row++, "Name: %s", name.c_str());
	print(row++, "Health: %d", health);

	unordered_map<string, string>::iterator it = properties.begin();
	while (it != properties.end()) {
		print(row++, "%s: %s", it->first.c_str(), it->second.c_str());
		it ++;
	}

	row += 2;
	print(row, "(enter)");

	refreshScreen();

	input(row, 4, ALLOW_EMPTY);
}
