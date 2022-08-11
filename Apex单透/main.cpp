#include "Tools/verify.h"
#include "function.h"
#include "Driver/MemOperations.h"
#include <direct.h>
#include <string>
#include <stringapiset.h>
#include <codecvt>
#include "Tools/version.hpp"
#include "Tools/curl.hpp"

bool active = true, debug = false;
bool k_f5 = 0;
bool k_f6 = 0;
bool k_f7 = 0;

bool player_glow = true;
bool show_shield = true;

bool show_watcher = true;


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


bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}
void HotKey() {
	while (1) {

		/*if (IsKeyDown(VK_F4))
		{
			active = false;
		}*/

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
		}/**/

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
	while (true)
	{
		Offset::localEntity = read<DWORD_PTR>(dwProcess_Base + OFFSET_LOCAL_ENT);

		std::vector<player> TmpPlayerList = {};
		std::vector<watcher> TmpWatcherList = {};
		player tmpPlayer;
		watcher tmpWatcher;
		int localPlayerTeamID = read<int>(Offset::localEntity + OFFSET_TEAM);
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
					int playerTeamID = read<int>(Entity + OFFSET_TEAM);
					int shield_type = read<int>(Entity + OFFSET_SHIELD_TYPE);
					GetName(dwProcess_Base, Entity, tmpPlayer.name);

					int dead = read<int>(Entity + OFFSET_LIFESTATE);//dead == 1，player is dead
					if (dead == 1) {
						/*float angle = read<float>(Entity + OFFSET_yYaw);
						float local_angle = read<float>(Offset::localEntity + OFFSET_yYaw);

						float yaw = -angle; // yaw is inverted
						if (yaw < 0)
							yaw += 360; yaw += 90; // yaw is off by 90
						if (yaw > 360)
							yaw -= 360;
						//local
						float local_yaw = -local_angle; // yaw is inverted
						if (local_yaw < 0)
							local_yaw += 360; local_yaw += 90; // yaw is off by 90
						if (local_yaw > 360)
							local_yaw -= 360;
						*/
						float yaw = GetYaw(Entity);
						float local_yaw = GetYaw(Offset::localEntity);

						if (yaw == local_yaw) {
							strcpy(tmpWatcher.name, tmpPlayer.name);
							TmpWatcherList.push_back(tmpWatcher);
						}
					}

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

						TmpPlayerList.push_back(tmpPlayer);
						
						//}
					}
				}
			}
		}
		watcherList = TmpWatcherList;
		Offset::players = TmpPlayerList;
		Offset::localTeamID = localPlayerTeamID;
		TmpPlayerList.clear();
		TmpWatcherList.clear();

		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
	while (true)
	{
		auto tmpWathcerList = watcherList;
		std::string str = "当前您的观众数:"+std::to_string(tmpWathcerList.size());

		printf("%s\n", str.c_str());
		printf("-----------------------------------------------\n");
		for (int i = 0; i < tmpWathcerList.size(); i++) {
			printf("%s\n", Utf8ToGb2312(tmpWathcerList[i].name));//获取到的名字是u8编码的，需要转换为命令行可以显示的编码格式，中文系统默认为936（简体中文）
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		system("cls");
	}
}

int main(int argCount, char** argVector)
{
	bool QR = false;
	version ver;
	string latestVer,thisVer;
	thisVer = get_thisVersion();
	cout << "当前版本：" << thisVer << endl;
	std::string Path = current_working_directory();
	IsDebug(Path);
	if (debug)
	{
		QR = true;
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
			//system("cls");
			cout << "当前版本：" << thisVer << endl<<"最新版本："<<latestVer<<endl;
			cout << "请升级最新版本" << endl;
			getchar();
			QR = false;
			return 1;
		}
		else {
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
		system(E("CLS"));
		printf(E("[!] 正在获取游戏进程... \n"));
		while (!hwnd)
		{
			hwnd = FindWindowA(NULL, ("Apex Legends"));
			Sleep(500);
		}
		if (int returnvalue = Init()) //driver handle and process Ids
		{
			if (returnvalue == DRIVER_NOT_LOADED)
				std::cout << "Driver not loaded\n";
			if (returnvalue == PROCESS_NOT_FOUND)
				std::cout << "Process not found\n";
			system("pause");
			return 0;
		}

		while (!(dwProcessId = Util::GetProcessId(dwProcessName)))
			Sleep(1000);


		//获取基模块地址
		while (dwProcess_Base == 0)
		{
			dwProcess_Base = get_module_base(dwProcessName);
			Sleep(1000);
		}
		//system("pause");
		system(E("CLS"));
		printf((" [+] Contact newton_miku\n [+]啊，哈哈哈哈\n [+]寄汤来喽 \n"));
		printf(" 进程名称: %s \n 进程ID: %d \n 基地址: 0x%llx\n", dwProcessName, dwProcessId, dwProcess_Base);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(HotKey), nullptr, NULL, nullptr);
		//CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(GameCache), nullptr, NULL, nullptr);
		std::thread gamecache_th(GameCache);
		gamecache_th.detach();
		std::thread glow_th(PlayerGlowFunc);
		glow_th.detach();
		//CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(PlayerGlowFunc), nullptr, NULL, nullptr);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(WathcerFunction), nullptr, NULL, nullptr);
		//system("chcp 65001");
		system(E("CLS"));
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		MessageBox(NULL, ("  程序已加载完毕\n[F4]退出程序\n[F5]开关敌人发光\n[F6]开关护甲颜色发光\n[F7]显示观众列表"), "仅供内部使用", MB_OK);
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
				exit(0);
				return 0;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
		exit(0);
		return 0;
	}
}
