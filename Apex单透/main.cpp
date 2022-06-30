#include "function.h"
#include "Driver/MemOperations.h"
#include <direct.h>
#include "QR_Verify.hpp"

bool active = true, debug = false;
bool k_f5 = 0;
bool k_f6 = 0;
bool k_f7 = 0;

bool player_glow = true;
bool show_shield = true;

bool item_glow = false;

bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}
void HotKey() {
	while (1) {

		if (IsKeyDown(VK_F4))
		{
			active = false;
		}

		if (IsKeyDown(VK_F5) && k_f5 == 0)//F5 敌人发光
		{
			k_f5 = 1;
			player_glow = !player_glow;
		}
		else if (!IsKeyDown(VK_F5) && k_f5 == 1)
		{
			k_f6 = 0;
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

		/*
		if (IsKeyDown(VK_F7) && k_f7 == 0)//F7 物品发光
		{
			k_f7 = 1;
			item_glow = !item_glow;
		}
		else if (!IsKeyDown(VK_F7) && k_f7 == 1)
		{
			k_f7 = 0;
		}/**/

	}
}

void GameCache()
{
	while (true)
	{
		Offset::localEntity = read<DWORD_PTR>(dwProcess_Base + Offset::LocalPlayer);

		std::vector<DWORD_PTR> playerTmpList = {};
		std::vector<player> TmpPlayerList = {};
		player tmpPlayer;
		
		for (int i = 0,j = 0; i < 64; i++)
		{
			//DWORD_PTR Entity = read<DWORD_PTR>(dwProcess_Base + Offset::cl_entitylist + (i * 0x20));
			DWORD_PTR Entity = GetEntityById(i, dwProcess_Base);
			if (Entity == NULL) continue;
			//printf("Entity：0x%p\n", Entity);

			std::string signifer = Util::GetSignifier(Entity);

			if (signifer == E("player"))
			{
				//判断是否不是本地玩家
				if (Offset::localEntity != Entity)
				{
					int localPlayerTeamID = read<int>(Offset::localEntity + Offset::m_iTeamNum);
					int playerTeamID = read<int>(Entity + Offset::m_iTeamNum);
					int shield_type = read<int>(Entity + OFFSET_SHIELD_TYPE);

					//判断是否是敌人
					//if (playerTeamID != localPlayerTeamID)
					if(1)
					{
						//int lifeState = read<int>(Entity + Offset::m_lifeState);

						//lifeState等于0，表示此玩家活着的
						//if (lifeState == 0)
						//{
						tmpPlayer.Entity = Entity;
						tmpPlayer.shield_type = shield_type;
						tmpPlayer.teamid = playerTeamID;

						playerTmpList.push_back(Entity);
						TmpPlayerList.push_back(tmpPlayer);
						
						//}
					}
				}
			}
		}
		Offset::playerList = playerTmpList;
		Offset::players = TmpPlayerList;
		Offset::localTeamID = read<int>(Offset::localEntity + Offset::m_iTeamNum);
		playerTmpList.clear();
		TmpPlayerList.clear();

		Sleep(200);
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
	/*if (NULL == (debugFile = fopen(path.c_str(), "r")))
		return;
	else {
		fclose(debugFile);
		debug = true;
	}*/
	if (NULL == (debugFile = fopen(Cdebug.c_str(), "r")))
		return;
	else {
		fclose(debugFile);
		debug = true;
	}

}

int main(int argCount, char** argVector)
{
	bool QR = false;
	std::string Path = current_working_directory();

	IsDebug(Path);
	if (debug)
	{
		QR = true;
	}
	else {
		printf("请使用QQ扫码\n");
		int QR_Back = QR_Verify(Path);
		if (QR_Back != 0)
		{
			QR = false;
			return 1;
		}
		else if (QR_Back == 0)
			QR = true;
	}

	if (QR)
	{
		//SetConsoleTitleA(Util::RandomString(16).c_str());
		srand(time(NULL));
		std::string filePath = argVector[0];
		Util::RenameFile(filePath);
		system(E("CLS"));
		while (!hwnd)
		{
			hwnd = FindWindowA(NULL, ("Apex Legends"));
			Sleep(500);
		}
		printf(E("[!] 正在获取游戏进程... \n"));

		while (!(dwProcessId = Util::GetProcessId(dwProcessName)))
			Sleep(1000);


		if (int returnvalue = Init()) //driver handle and process Ids
		{
			if (returnvalue == DRIVER_NOT_LOADED)
				std::cout << "Driver not loaded\n";
			if (returnvalue == PROCESS_NOT_FOUND)
				std::cout << "Process not found\n";
			system("pause");
			return 0;
		}
		//获取基模块地址
		while (dwProcess_Base == 0)
		{
			dwProcess_Base = get_module_base(dwProcessName);
			Sleep(1000);
		}
		//system("pause");
		system(E("CLS"));
		printf((" [+] Contact newton_miku\n [+]啊，哈哈哈哈\n [+]寄汤来喽 \n"));
		printf(" 进程名称: %s \n 进程ID: %d \n 基地址: %llx\n", dwProcessName, dwProcessId, dwProcess_Base);

		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(HotKey), nullptr, NULL, nullptr);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(GameCache), nullptr, NULL, nullptr);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(PlayerGlowFunc), nullptr, NULL, nullptr);
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		MessageBox(NULL, ("  程序已加载完毕\n[F4]退出程序\n[F5]开关敌人发光\n[F6]开关护甲颜色发光\n"), "仅供内部使用", MB_OK);
		/*bool WindowFocus = false;
		while (WindowFocus == false)
		{
			DWORD ForegroundWindowProcessID;
			GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
			if (Util::GetProcessId(dwProcessName) == ForegroundWindowProcessID)
			{

				gameHWND = GetForegroundWindow();

				RECT TempRect;
				GetClientRect(gameHWND, &TempRect);
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
		while (TRUE)
		{
			RECT TempRect;

			GetClientRect(gameHWND, &TempRect);

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
		}/**/
		while (hwnd && active)
		{
			hwnd = FindWindowA(NULL, ("Apex Legends"));
			if (!hwnd)
			{
				exit(0);
				return 0;
			}
			std::this_thread::sleep_for(std::chrono::seconds(30));
		}
		exit(0);
		return 0;
	}
}
