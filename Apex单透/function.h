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

#ifndef _FUNCTION_H
#define _FUNCTION_H
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
#endif

#ifndef _DRIVE
#include "Driver/MemOperations.h"
#endif 

#ifndef _VECTOR_H
#include "vector.h"
#endif
#ifndef _CONFIG_H
#include "config.h"
#endif

#ifndef _OFFSET_H
#include "offset.h"
#endif

#ifndef _UTIL_H
#include "util.h"
#endif
/*
#ifndef _AIMASSIST_HPP
#include "Header Files/aimassist.hpp"
#endif

//#ifndef _AIMBOT_HPP
#include "Header Files/aimbot.hpp"
//#endif

#ifndef _AIMBOT_CPP
#include "aimbot.cpp""
#endif

#ifndef _GLOW_H
#include"glow.h"
#endif
/**/