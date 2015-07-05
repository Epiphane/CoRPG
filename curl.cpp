#include <iostream>
#include <curl/curl.h>
#include <cstring>

#include "curl.h"

using namespace std;

char CurlBuffer[2048];
int CurlBufferNdx = 0;
size_t write_data(void *buf, size_t size, size_t nmemb, void *userp) {
	memcpy(&CurlBuffer[CurlBufferNdx], buf, size * nmemb);
	CurlBufferNdx += size * nmemb;
	CurlBuffer[CurlBufferNdx] = 0;

	return size * nmemb;
}

inline Json::Value json_perform(CURL *handle, string url) {
	Json::Value result;
	CurlBufferNdx = 0;
	if (curl_easy_perform(handle) != CURLE_OK) {
		cerr << "Error sending request to " << url << endl;
	}
	else {
		Json::Reader *reader = new Json::Reader();
		reader->parse(CurlBuffer, &CurlBuffer[CurlBufferNdx], result, false);
	}

	return result;
}

Json::Value Curl::GET(string url) {
	url = API_BASE + url;

	CURL *handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

	return json_perform(handle, url);
}

Json::Value Curl::GET(string url, vector<string> deps) {
	url = API_BASE + url + "?deps=";

	vector<string>::iterator it = deps.begin();
	for (;it < deps.end(); it ++) {
		url += *it + ",";
	}

	CURL *handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);

	return json_perform(handle, url);
}

void Curl::POST(GameObject *obj) {
	
}

void Curl::PUT(GameObject *obj) {
	
}
