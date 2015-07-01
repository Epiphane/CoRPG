#ifndef CURL_H
#define CURL_H 

#include <string>
#include <vector>
#include "gameinfo.h"
#include "json/json.h"

namespace Curl {
	Json::Value GET(std::string url);
//	GameObject *GET(std::string name);
	GameObject *GET(std::string name, std::vector<std::string> dependencies);
	void POST      (GameObject *newObject);
	void PUT       (GameObject *object);
};

#endif /* CURL_H */
