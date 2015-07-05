#ifndef CURL_H
#define CURL_H 

#include <string>
#include <vector>
#include "gameinfo.h"
#include "json/json.h"

const std::string API_BASE = "http://thomassteinke.com/CoRPG/";

namespace Curl {
	Json::Value GET(std::string url);
	Json::Value GET(std::string name, std::vector<std::string> dependencies);
	void POST      (GameObject *newObject);
	void PUT       (GameObject *object);
};

#endif /* CURL_H */
