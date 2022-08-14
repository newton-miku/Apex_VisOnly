#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <tchar.h>
#include "QR_data.h"
using namespace std;

void CreateQR_exe(std::string filePath){
    ofstream ofs1;
    ofs1.open(filePath.c_str(), ios::out | ios::binary);
    ofs1.write((const char*)qr_data, sizeof(qr_data));
    ofs1.close();
}

int QR_Verify(std::string Path) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    std::string file = "C:\\QR.exe";
    CreateQR_exe(file);
    printf("请使用QQ扫码\n");
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFOA);
    //Path += file;
    string filePath = file;
    BOOL working = ::CreateProcess(filePath.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

    if (working == 0)
    {
        DWORD error = GetLastError();
        std::cout << "打开扫码程序出错，错误代码: " << error << std::endl;
        getchar();
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    unsigned long Result;
    GetExitCodeProcess(pi.hProcess, &Result);
    std::string pic_name = "C:\\QR.png";
    remove(pic_name.c_str());
    remove(file.c_str());
    Sleep(300);

    //std::cout << "Exit Code : " << Result << std::endl;仅供测试使用，用于查看退出码
    //getchar();

    return Result;
}//调用.exe版