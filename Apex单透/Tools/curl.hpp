#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <curl\curl.h>
#include <json\json.h>
using namespace std;

#define GET_PNG		0
#define GET_JSON	1

std::string getResponseStr;
long httpcode;//网页状态码
FILE* fp;//QR文件指针

#define VER_GET_ERR		1
#define VER_GET_OK		0

typedef struct version {
	int get;
	string ver;
};

//get请求和post请求数据响应函数
size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
{
	//在注释的里面可以打印请求流，cookie的信息
	//cout << "----->reply" << endl;
	string* str = (string*)stream;
	//cout << *str << endl;
	(*str).append((char*)ptr, size * nmemb);
	return size * nmemb;
}
//写入文件函数
size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
	int written = fwrite(ptr, size, nmemb, (FILE*)fp);
	return written;
}

CURLcode curl_get_req(const std::string& url, std::string& response, int type)
{
	//curl初始化  
	CURL* curl = curl_easy_init();
	// curl返回值 
	CURLcode res;
	if (curl)
	{
		//设置curl的请求头
		struct curl_slist* header_list = NULL;
		header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

		//接收响应头数据0代表不接收 1代表接收
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);

		//设置请求的URL地址 
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		//获取网页请求状态码

		//允许几次重定向
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);

		//设置ssl验证
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);

		//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
			//这里的pem文件是http://curl.haxx.se/ca/cacert.pem
		//curl_easy_setopt(curl, CURLOPT_CAINFO, "./cacert.pem");

		//CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

		//设置数据接收函数
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
		switch (type) {
		case GET_PNG:
			fp = fopen(response.c_str(), "wb");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			break;
		case GET_JSON:
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response); break;
		}

		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0);

		//设置超时时间
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time  
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

		// 开启请求  
		res = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpcode);
		//cout << "httpcode:" << httpcode << endl;
	}
	// 释放curl 
	curl_easy_cleanup(curl);
	return res;
}
string set_version(string getResponseStr,int source) {
	Json::Reader reader;
	Json::Value value;
	string out;
	if (reader.parse(getResponseStr, value))
	{
			out = value["ret"].asString();
			if (strcmp(out.c_str(), "200") == 0) {
				const Json::Value arrayObj = value["data"];
				for (unsigned int i = 0; i < arrayObj.size(); i++)
				{
					out = arrayObj["version"].asString();
				}
				return out;
			}
			else return ("0.0.0.0");
	}
}
version get_latestVersion() {
	int source = 0;
	string url = "http://ddxnb.tk:8000/?s=App.Version.now";
	string url1 = "https://www.ddxnb.tk/version.php?type=product";
	string getResponseStr,verStr;
	version ver;
	auto res = curl_get_req(url, getResponseStr, GET_JSON);
	if ( res == CURLE_OK) {
		if (httpcode != 200) {
			source = 1;
		}
	}
	else {
		source = 1;
	}
	if (source == 1)
	{
		source = 1;
		res = curl_get_req(url1, getResponseStr, GET_JSON);
		if (res != CURLE_OK) {
			ver.get = VER_GET_ERR;
			ver.ver = "1.1.1.1";
			return ver;
		}
	}
	verStr = set_version(getResponseStr,source);
	ver.get = VER_GET_OK;
	ver.ver = verStr;
	return ver;
}