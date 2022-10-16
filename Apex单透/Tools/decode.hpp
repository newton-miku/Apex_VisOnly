#pragma once
#include <stringapiset.h>
#include <string>
#include <codecvt>
std::wstring UTF8ToWide(const std::string& source)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(source);
}

std::string WideToAscii(std::wstring _strSrc)
{
	int ansiiLen = WideCharToMultiByte(CP_ACP, 0, _strSrc.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pAssii = (char*)malloc(sizeof(char) * ansiiLen);
	WideCharToMultiByte(CP_ACP, 0, _strSrc.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}

std::string UTF8ToAscii(std::string _strSrc)
{
	return WideToAscii(UTF8ToWide(_strSrc));
}

char* Utf8ToGb2312(const char* p)                                               //unicode转国标
{
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, NULL, 0);
	char* psText;                                                         //存储文件名
	wchar_t* pwText = (wchar_t*)malloc(dwNum * sizeof(wchar_t));
	dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, pwText, dwNum);
	dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL);
	psText = (char*)malloc(dwNum * sizeof(char));
	dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, psText, dwNum, NULL, NULL);
	free(pwText);
	return psText;
}

std::string WStr2Str(std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char* _Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring Str2WStr(const std::string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t* _Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}
