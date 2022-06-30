#pragma once
#include <iostream>
#include <windows.h>
#include <tchar.h>

int QR_Verify(std::string Path) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFOA);
    std::string file = "\\QR\\QR.exe";
    Path += file;
    BOOL working = ::CreateProcess(Path.c_str(), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

    if (working == 0)
    {
        DWORD error = GetLastError();
        std::cout << "CreateProcess Error : " << error << std::endl;
        getchar();
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    unsigned long Result;
    GetExitCodeProcess(pi.hProcess, &Result);
    std::string pic_name = "QR.png";
    remove(pic_name.c_str());
    Sleep(300);

    //std::cout << "Exit Code : " << Result << std::endl;仅供测试使用，用于查看退出码
    //getchar();

    return Result;
}//调用.exe版