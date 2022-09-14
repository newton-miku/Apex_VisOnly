#include <windows.h>
#include <dwmapi.h>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include<thread>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")

const char* dwProcessName = "r5apex.exe";
DWORD dwProcessId = NULL;
uint64_t dwProcess_Base = NULL;
HWND gameHWND = NULL;

int ScreenWidth = 1920; //窗口宽度
int ScreenHeight = 1080; //窗口高度
int ScreenLeft = NULL; //窗口左侧
int ScreenRight = NULL; //窗口右侧
int ScreenTop = NULL; //窗口顶部
int ScreenBottom = NULL; //窗口底部

//#include "xor.h"
#include "vector.h"
#include "config.h"
#include "offset.h"
#include "util.h"
#include "glow.h"
#include "aimbot.h"

