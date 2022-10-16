#pragma once
#include "Tools/verify.h"
#include <direct.h>
#include <stringapiset.h>
#include <codecvt>
#include "Tools/version.hpp"
#include "Tools/curl.hpp"
#include "Tools/Load Driver/Load_Drive.hpp"
#include "Tools/websocket/webShow.hpp"
#include "glow.h"
#include "aimbot.cpp"
#include "aimassist.cpp"
#include "function.h"

#define _AMD64_

#define BR "<br>"
bool active = true;

// 更改控制台设置，防止控制台属性导致的判定失败
void setmode()
{
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE; //移除快速编辑模式
	mode &= ~ENABLE_INSERT_MODE;     //移除插入模式
	// mode &= ~ENABLE_MOUSE_INPUT;     //移除鼠标输入
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}
void HotKey() {
	while (QR) {

		if (IsKeyDown(VK_F4))
		{
			QR = false;
		}

		if (IsKeyDown(VK_F5) && k_f5 == 0)//F5 敌人发光
		{
			k_f5 = 1;
			player_glow = !player_glow;
		}
		else if (!IsKeyDown(VK_F5) && k_f5 == 1)
		{
			k_f5 = 0;
		}
		
		if (IsKeyDown(VK_F6) && k_f6 == 0)//F6 显示护甲
		{
			k_f6 = 1;
			show_shield = !show_shield;
		}
		else if (!IsKeyDown(VK_F6) && k_f6 == 1)
		{
			k_f6 = 0;
		}

		if (IsKeyDown(VK_F7) && k_f7 == 0)//F7 观众列表
		{
			k_f7 = 1;
			show_watcher = !show_watcher;
		}
		else if (!IsKeyDown(VK_F7) && k_f7 == 1)
		{
			k_f7 = 0;
		}

		if (IsKeyDown(VK_F8) && k_f8 == 0)//F8 物品发光
		{
			k_f8 = 1;
			item_glow = !item_glow;
		}
		else if (!IsKeyDown(VK_F8) && k_f8 == 1)
		{
			k_f8 = 0;
		}
		
		if (IsKeyDown(VK_LEFT) && k_left == 0)//←减小平滑
		{
			k_left = 1;
			if (Config::AimbotSmooth <= 1) {
				continue;
			}
			else
			{
				Config::AimbotSmooth --;
			}
		}
		else if (!IsKeyDown(VK_LEFT) && k_left == 1)
		{
			k_left = 0;
		}
		if (IsKeyDown(VK_RIGHT) && k_right == 0)//→ 增加平滑
		{
			k_right = 1;
			if (Config::AimbotSmooth >= 30) {
				continue;
			}
			else
			{
				Config::AimbotSmooth++;
			}
		}
		else if (!IsKeyDown(VK_RIGHT) && k_right == 1)
		{
			k_right = 0;
		}
		if (IsKeyDown(VK_UP) && k_up == 0)//↑ 增加FOV
		{
			k_up = 1;
			if (Config::AimbotFOV >= 120) {
				continue;
			}
			else
			{
				Config::AimbotFOV +=10;
			}
		}
		else if (!IsKeyDown(VK_UP) && k_up == 1)
		{
			k_up = 0;
		}
		if (IsKeyDown(VK_DOWN) && k_down == 0)//↓ 减小FOV
		{
			k_down = 1;
			if (Config::AimbotFOV <= 30) {
				continue;
			}
			else
			{
				Config::AimbotFOV -=10;
			}
		}
		else if (!IsKeyDown(VK_DOWN) && k_down == 1)
		{
			k_down = 0;
		}
		/**/

	}
}
float GetYaw(DWORD_PTR Entity) {
	float yaw = 0;
	yaw = read<float>(Entity + OFFSET_YAW);

	if (yaw < 0)
		yaw += 360;
	yaw += 90;
	if (yaw > 360)
		yaw -= 360;

	return yaw;
}

void GameCache()
{
	while (QR)
	{
		Offset::localEntity = read<uintptr_t>(oBaseAddress + OFFSET_LOCAL_ENT);
		std::vector<player> TmpPlayerList = {};
		std::vector<watcher> TmpWatcherList = {};
		player tmpPlayer;
		watcher tmpWatcher;
		int localPlayerTeamID = GetTeam(Offset::localEntity);
		for (int i = 0,j = 0; i < 64; i++)
		{
			//DWORD_PTR Entity = read<DWORD_PTR>(dwProcess_Base + Offset::cl_entitylist + (i * 0x20));
			uintptr_t Entity = GetEntityById(i, oBaseAddress);
			if (Entity == NULL) continue;
			//printf("Entity：0x%p\n", Entity);
			std::string signifer = Util::GetSignifier(Entity);
			if (0)
			if (signifer == ("npc_dummie")) {
				Vector3 a =  Util::GetEntityBonePosition(Entity, 7);
				cout << "a:" <<  a.x <<"\t" << a.y << "\t" << a.z << endl;
			}
			if (signifer == "player") //|| signifer == "npc_dummie")
			{
				//cout << i<<signifer << endl;
				//判断是否不是本地玩家
				if (Offset::localEntity != Entity)
				{
					int playerTeamID = GetTeam(Entity);
					int shield_type = GetShieldType(Entity);
					GetName(oBaseAddress, Entity, tmpPlayer.name);

					int dead = read<int>(Entity + OFFSET_LIFESTATE);//dead == 1，player is dead
					if (dead) {
						float yaw = GetYaw(Entity);
						float local_yaw = GetYaw(Offset::localEntity);
						if (yaw == local_yaw) {
							tmpWatcher.team = playerTeamID;
							strcpy(tmpWatcher.name, tmpPlayer.name);
							TmpWatcherList.push_back(tmpWatcher);
						}
					}

					//判断是否是敌人
					//if (playerTeamID != localPlayerTeamID)
					if(1)
					{
						tmpPlayer.i = i;
						tmpPlayer.dead = dead;
						tmpPlayer.Entity = Entity;
						tmpPlayer.shield_type = shield_type;
						tmpPlayer.teamid = playerTeamID;

						TmpPlayerList.push_back(tmpPlayer);
					}
				}
			}
		}
		watcherList = TmpWatcherList;
		Offset::players = TmpPlayerList;
		Offset::localTeamID = localPlayerTeamID;
		TmpPlayerList.clear();
		TmpWatcherList.clear();

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

std::string current_working_directory()
{
	char buff[250];
	_getcwd(buff, 250);
	std::string current_working_directory(buff);
	return current_working_directory;
}
void IsDebug(std::string path) {
	FILE* debugFile;
	path += "\\debug.txt";
	std::string Cdebug = "C:\\debug.txt";
	if (NULL == (debugFile = fopen(Cdebug.c_str(), "r")))
		return;
	else {
		fclose(debugFile);
		debug = true;
	}

}

void WathcerFunction() {
	while (QR)
	{
		auto tmpWathcerList = watcherList;
		std::string str = "当前您的观众数:"+std::to_string(tmpWathcerList.size());
		string webStr = str + BR + "-----------------------------------------------";
		string tmpname;
		if (Config::EnableAimbot)
		{
			cout << "[上下]\tFOV:" << Config::AimbotFOV << endl << "[左右]\t平滑：" << Config::AimbotSmooth << endl;
		}
		if(tmpWathcerList.size()>0)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
		else
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
		printf("%s\n", str.c_str());
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf("-----------------------------------------------\n");
		if (tmpWathcerList.size() > 0) {
			for (auto tmpWatcher : tmpWathcerList) {
				tmpname = tmpWatcher.name;
				printf("Team%d %s\n", tmpWatcher.team, Utf8ToGb2312(tmpname.c_str()));//获取到的名字是u8编码的，需要转换为命令行可以显示的编码格式，中文系统默认为936（简体中文）
				webStr += BR + tmpname;
			}
		}
		sendWebMsg(webStr);
		std::this_thread::sleep_for(chrono::seconds(1));
		system("cls");
	}
}
int IsRunning() {
	while (hwnd && !GetAsyncKeyState(VK_F4))
	{
		if (show_watcher) {
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		}
		else
		{
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}
		hwnd = FindWindowA(NULL, ("Apex Legends"));
		if (!hwnd)
		{
			running = false;
			exit(0);
			return 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}
void set_window() {
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //获取标准输出句柄
	//SetConsoleTextAttribute(hOutput, FOREGROUND_GREEN | FOREGROUND_INTENSITY); //设置文本颜色绿色
	SetConsoleTextAttribute(hOutput, 160); //设置文本颜色黑字绿底 https://cloud.tencent.com/developer/article/1732036
	 
	SetLayeredWindowAttributes(GetConsoleWindow(), RGB(12, 12, 12), 0, LWA_COLORKEY);//默认黑窗口的透明
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 600, 400, SWP_NOMOVE);//设置窗口置顶
}
void GetScreen() {
	bool WindowFocus = false;
	gameHWND=hwnd;
	while (WindowFocus == false)
	{
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (Util::GetProcessId(dwProcessName) == ForegroundWindowProcessID)
		{

			hwnd = GetForegroundWindow();

			RECT TempRect;
			GetClientRect(hwnd, &TempRect);
			ScreenWidth = TempRect.right;
			ScreenHeight = TempRect.bottom;
			ScreenLeft = TempRect.left;
			ScreenRight = TempRect.right;
			ScreenTop = TempRect.top;
			ScreenBottom = TempRect.bottom;

			WindowFocus = true;
		}
	}

	static RECT OldRect;
	while (QR)
	{
		RECT TempRect;

		GetClientRect(hwnd, &TempRect);

		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom)
		{
			OldRect = TempRect;
			ScreenWidth = TempRect.right;
			ScreenHeight = TempRect.bottom;
			ScreenLeft = TempRect.left;
			ScreenRight = TempRect.right;
			ScreenTop = TempRect.top;
			ScreenBottom = TempRect.bottom;
		}
	}
}
int main(int argCount, char** argVector)
{
	bool verOK=false;
	version ver;
	string latestVer,thisVer;
	thisVer = get_thisVersion();
	cout << "当前版本：" << thisVer << endl;
	std::string Path = current_working_directory();
	IsDebug(Path);
	//debug = true;//对外debug版本
	if (debug)
	{
		QR = true;
		item_glow = true;
	}
	else {
		ver = get_latestVersion();
		//在这里写获取最新版本 http://ddxnb.tk:8000/?s=App.Version.now
		//或者使用 https://www.ddxnb.tk/version.php?type=product
		
		//这里是检查最新版本
		if (ver.get == VER_GET_ERR) {
			cout<<"服务器版本验证出错，请联系管理员\n"<<endl;
			getchar();
			return 1;
		}
		else if (ver.get == VER_GET_OK) {
			latestVer = ver.ver;
		}
		if (strcmp(thisVer.c_str(), latestVer.c_str()) != 0) {
			if (strcmp("3.9.3.9", latestVer.c_str()) == 0)
			{
				cout << "当前版本：" << thisVer << endl << "当前为开放版本模式，暂时不做在线版本验证" << endl;
				verOK = true;
			}
			else
			{
				cout << "当前版本：" << thisVer << endl << "最新版本：" << latestVer << endl;
				cout << "请升级最新版本" << endl;
				verOK = false;
				getchar();
				QR = false;
				return 1;
			}
		}
		else
		{
			verOK = true;
		}
		if(verOK) {
			//以下为验证程序
			int Verify_return = verify(Path);
			if (Verify_return != 1)
			{
				QR = false;
				return 1;
			}
			else if (Verify_return == 1)
				QR = true;
			//int QR_Back = QR_Verify(Path);
			/*if (QR_Back != 0)
			{
				QR = false;
				return 1;
			}
			else if (QR_Back == 0)
				QR = true;
			/**/
		}
	}

	if (QR)
	{
		//SetConsoleTitleA(Util::RandomString(16).c_str());
		srand(time(NULL));
		std::string filePath = argVector[0];
		Util::RenameFile(filePath);
		loadDriver();
		system(("CLS"));
		set_window();
		printf(("[!] 正在获取游戏进程... \n"));
		while (!hwnd)
		{
			hwnd = FindWindowA(NULL, ("Apex Legends"));
			Sleep(500);
		}
		if (int returnvalue = Init()) //driver handle and process Ids
		{
			if (returnvalue == DRIVER_NOT_LOADED)
				std::cout << "Driver not loaded\n";
			system("pause");
			return 1;
			if (returnvalue == PROCESS_NOT_FOUND) {
				std::cout << "Process not found\n";
				while (!GAME_PROCESS_ID)
					GAME_PROCESS_ID = GetProcessIdByName(GAME_PROCESS_NAME);
			}
		}

		while (!(dwProcessId = Util::GetProcessId(dwProcessName)))
			Sleep(1000);


		//获取基模块地址
		while (dwProcess_Base == 0)
		{
			dwProcess_Base = get_module_base(dwProcessName);
			Sleep(1000);
		}
		oBaseAddress = dwProcess_Base;
		thread ser_th(serverFunc);
		ser_th.detach();
		system(("CLS"));
		printf((" [+] Contact newton_miku\n [+]啊，哈哈哈哈\n [+]寄汤来喽 \n"));
		printf(" 进程名称: %s \n 进程ID: %d \n 基地址: 0x%llx\n", dwProcessName, dwProcessId, dwProcess_Base);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(HotKey), nullptr, NULL, nullptr);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Aimbot::Run), nullptr, NULL, nullptr);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(AimAssistThread), nullptr, NULL, nullptr);
		std::thread gamecache_th(GameCache);	gamecache_th.detach();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(WathcerFunction), nullptr, NULL, nullptr);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(ItemGlowThread), nullptr, NULL, nullptr);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(PlayerGlowThread), nullptr, NULL, nullptr);
		//system("chcp 65001");
		system(("CLS"));
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		MessageBox(NULL, ("  程序已加载完毕\n[F4]退出程序\n[F5]开关敌人发光\n[F6]开关护甲颜色发光\n[F7]显示观众列表"), "仅供内部使用", MB_OK);
		/**/
		//webShow();
		//cout << get_time() << endl;
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(GetScreen), nullptr, NULL, nullptr);
		setmode();
		while (hwnd && QR)
		{
			if (show_watcher) {
				ShowWindow(GetConsoleWindow(), SW_SHOW);
			}
			else
			{
				ShowWindow(GetConsoleWindow(), SW_HIDE);
			}
			hwnd = FindWindowA(NULL, ("Apex Legends"));
			if (!hwnd||!QR)
			{
				running = false;
				//system("taskkill /f /im Apex_VisOnly.exe");
				exit(0);
				return 0;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
		exit(0);
		return 0;
	}
}
