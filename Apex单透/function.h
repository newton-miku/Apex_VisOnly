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

int ScreenWidth = 1920; //���ڿ��
int ScreenHeight = 1080; //���ڸ߶�
int ScreenLeft = NULL; //�������
int ScreenRight = NULL; //�����Ҳ�
int ScreenTop = NULL; //���ڶ���
int ScreenBottom = NULL; //���ڵײ�

//#include "xor.h"
#include "vector.h"
#include "config.h"
#include "offset.h"
#include "util.h"
#include "glow.h"
#include "aimbot.h"

