#ifndef CURL_H
#define CURL_H 

#include <string>
#include <vector>
#include "gameinfo.h"
#include "json/json.h"

const std::string API_BASE = "http://thomassteinke.com/CoRPG/";

namespace Curl {
	Json::Value  GET(std::string url);
	Json::Value  GET(std::string name, std::vector<std::string> dependencies);
	Json::Value POST(Json::Value action);
	Json::Value  PUT(std::string name, std::string region, Json::Value state);
};

#endif /* CURL_H */
