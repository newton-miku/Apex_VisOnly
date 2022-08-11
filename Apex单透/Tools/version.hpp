#pragma once
#include <Windows.h>
#include <string>
#include <direct.h>
#include <iostream>

#pragma comment(lib, "version.lib")

using namespace std;

string GetExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	/*
	strrchr:函数功能：查找一个字符c在另一个字符串str中末次出现的位置（也就是从str的右侧开始查找字符c首次出现的位置），
	并返回这个位置的地址。如果未能找到指定字符，那么函数将返回NULL。
	使用这个地址返回从最后一个字符c到str末尾的字符串。
	*/
	//(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串//
	string path = szFilePath;
	return path;
}

LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}
//string转换车wstring
//推荐使用这个，在用.c_str()方法转换成LPCWSTR
//如果使用上面的方法，那么可能会由于名称中存在空格导致转换出错进而导致后续无法读取到版本号
std::wstring  StringToWString(const std::string& s)
{
	std::wstring wszStr;

	int nLength = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, NULL);
	wszStr.resize(nLength);
	LPWSTR lpwszStr = new wchar_t[nLength];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, lpwszStr, nLength);
	wszStr = lpwszStr;
	delete[] lpwszStr;
	return wszStr;
}
//返回文件版本号
//@params:filename:文件名
string GetFileVersion(LPCWSTR filename)
{
	string asVer = "";
	VS_FIXEDFILEINFO* pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	int iVerInfoSize = GetFileVersionInfoSizeW(filename, NULL);
	if (iVerInfoSize != 0)
	{
		char* pBuf = NULL;

		while (!pBuf)
		{
			pBuf = new char[iVerInfoSize];
		}
		if (GetFileVersionInfoW(filename, 0, iVerInfoSize, pBuf))
		{
			if (VerQueryValueA(pBuf, "\\", (void**)&pVsInfo, &iFileInfoSize))
			{
				sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwProductVersionMS), LOWORD(pVsInfo->dwProductVersionMS), HIWORD(pVsInfo->dwProductVersionLS), LOWORD(pVsInfo->dwProductVersionLS));//产品版本
				//sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));//文件版本
				asVer = pBuf;
			}
		}
		delete pBuf;
	}
	return asVer;
}

string get_thisVersion() {
	string path = GetExePath(), ver;
	ver = GetFileVersion(StringToWString(path).c_str());
	return ver;
}
