#include <iostream>
#include <curl/curl.h>
#include <cstring>

#include "curl.h"

using namespace std;

typedef struct curl_write_t {
	char *buf;
	int len, pos;
} curl_write_t;

size_t read_data(void *ptr, size_t size, size_t nmemb, void *userp) {
	curl_write_t *obj = (curl_write_t *)userp;

	int len = obj->len - obj->pos;
	if (size * nmemb < len)
		len = size * nmemb;

	memcpy(ptr, obj->buf + obj->pos, len);
	obj->pos += len;

	return len;
}

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

Json::Value Curl::POST(Json::Value action) {
	string url = API_BASE + "action";

	CURL *handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");

	string json = action.toStyledString();
	curl_write_t info;
	info.buf = new char[json.size() + 1];
	strcpy(info.buf, json.c_str());
	info.pos = 0;
	info.len = json.size();
	curl_easy_setopt(handle, CURLOPT_POST, 1);
	curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, info.len);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_data);
	curl_easy_setopt(handle, CURLOPT_READDATA, &info);
//	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, info.buf);

	return json_perform(handle, url);
}

Json::Value Curl::PUT(string name, string region, Json::Value state) {
	string url = API_BASE + region + "/" + name;

	CURL *handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, state.toStyledString().c_str());

	return json_perform(handle, url);
}
