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
long httpcode;//��ҳ״̬��
FILE* fp;//QR�ļ�ָ��

//get�����post����������Ӧ����
size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
{
	//��ע�͵�������Դ�ӡ��������cookie����Ϣ
	//cout << "----->reply" << endl;
	string* str = (string*)stream;
	//cout << *str << endl;
	(*str).append((char*)ptr, size * nmemb);
	return size * nmemb;
}
//д���ļ�����
size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
	int written = fwrite(ptr, size, nmemb, (FILE*)fp);
	return written;
}

CURLcode curl_get_req(const std::string& url, std::string& response, int type)
{
	//curl��ʼ��  
	CURL* curl = curl_easy_init();
	// curl����ֵ 
	CURLcode res;
	if (curl)
	{
		//����curl������ͷ
		struct curl_slist* header_list = NULL;
		header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

		//������Ӧͷ����0�������� 1�������
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);

		//���������URL��ַ 
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		//��ȡ��ҳ����״̬��
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpcode);

		//�������ض���
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);

		//����ssl��֤
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);

		//CURLOPT_VERBOSE��ֵΪ1ʱ������ʾ��ϸ�ĵ�����Ϣ
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

		//�������ݽ��պ���
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

		//���ó�ʱʱ��
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time  
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

		// ��������  
		res = curl_easy_perform(curl);
	}
	// �ͷ�curl 
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
string get_latestVersion() {
	int source = 0;
	string url = "http://ddxnb.tk:8000/?s=App.Version.now";
	string url1 = "https://www.ddxnb.tk/version.php?type=product";
	string getResponseStr,ver;
	auto res = curl_get_req(url, getResponseStr, GET_JSON);
	if ( res == CURLE_OK) {
		if (httpcode != 200) {
			source = 1;
			url = "http://www.ddxnb.tk/version.php?type=product";
			curl_get_req(url, getResponseStr, GET_JSON);
		}
	}
	else
	{
		source = 1;
		res = curl_get_req(url1, getResponseStr, GET_JSON);
		if (res != CURLE_OK) {
			printf("�������汾��֤��������ϵ����Ա\n");
			return ("1.1.1.1");
		}
	}
	ver = set_version(getResponseStr,source);
	return ver;
}