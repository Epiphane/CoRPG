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
	Json::Value  ACT(Json::Value action);
	Json::Value POST(std::string url, Json::Value object);
	Json::Value  PUT(std::string name, std::string region, Json::Value object);

	double getLatency();
};

#endif /* CURL_H */
