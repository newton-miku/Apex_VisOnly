#pragma once
#define _GLOW_H
#include "Tools/decode.hpp"
#include "Header Files/item.h"
#include "function.h"
using namespace std;

float playerglow1[3] = { 0.0f, 0.837104f, 0.056f };//可见敌人
float playerglow2[3] = { 0.0f, 0.0f, 1.0f };//可见倒地敌人
float playerglow3[3] = { 0.914416f, 0.004525f, 1.0f };//不可见敌人
float playerglow4[3] = { 0.0f, 0.0f, 1.0f };//不可见倒地敌人

float GREEN[3] = { 0.0f, 1.0f, 0.0f };
float RED[3] = { 1.0f, 0.0f, 0.0f };
float BLUE[3] = { 0.0f, 0.83529411f, 1.0f };
float PURPLE[3] = { 0.70588f, 0.0f, 1.0f };
float ORANGE[3] = { 1.0f, 0.6470588f, 0.0f };
float WHITE[3] = { 1.0f, 1.0f, 1.0f };
float GOLD[3] = { 1.0f, 0.933333f, 0.0f };



#define FREEDM_MODE		1
#define CONTROL_MODE	2

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

std::string GetGamemode(uintptr_t modBase) {
	uintptr_t gameModePtr = read<uintptr_t>(modBase + OFFSET_GAMEMODE);
	return  readStr(gameModePtr, 15);
}
std::string GetName(uintptr_t oBaseAddress, uintptr_t Entity, char name[])
{
	uintptr_t nameindex = read<uintptr_t>(Entity + 0x38);
	uintptr_t name_ptr = read<uintptr_t>(oBaseAddress + OFFSET_NAME_LIST + ((nameindex - 1) << 4));
	std::string names;
	names = readStr(name_ptr, 128);
	strcpy(name, names.c_str());
	return names;
}
int GetTeam(DWORD64 entity) {
	return read<int>(entity + OFFSET_TEAM);
}
int GetShieldType(DWORD64 entity) {
	return read<int>(entity + OFFSET_SHIELD_TYPE);
}
int GetShield(DWORD64 entity) {
	return read<int>(entity + OFFSET_SHIELD);
}
int GetHealth(DWORD64 entity) {
	return read<int>(entity + OFFSET_HEALTH);
}
int IsDead(DWORD64 Entity) {
	return read<int>(Entity + OFFSET_LIFESTATE); //dead == 1，player is dead
}
int IsDown(DWORD64 Entity) {
	return read<int>(Entity + OFFSET_BLEED_OUT_STATE); //down == 1，player is down
}

uintptr_t GetEntityById(int Ent, uintptr_t Base)
{
	uintptr_t EntityList = Base + OFFSET_ENTITYLIST; //updated
	uintptr_t BaseEntity = read<uintptr_t>(EntityList);
	if (!BaseEntity)
		return NULL;
	return  read<uintptr_t>(EntityList + (Ent << 5));
}


bool ColCheck(float a[], float b[]) {
	if (fabs(a[0] - b[0]) < 1e-6)
		if (fabs(a[1] - b[1]) < 1e-6)
			if (fabs(a[2] - b[2]) < 1e-6)
				return true;
	return false;
}

int CheckGameMode(string gamemode) {
	if (!strcmp(gamemode.c_str(), "freedm"))
	{
		Offset::freedm = true;
		Offset::control = false;
		return FREEDM_MODE;
	}
	if (!strcmp(gamemode.c_str(), "control")) {
		Offset::freedm = false;
		Offset::control = true;
		return CONTROL_MODE;
	}
	else {
		Offset::freedm = false;
		Offset::control = false;
		return 0;
	}
}

void player_glow_f(DWORD64 Entity, float* color)
{
	if (player_glow) {
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		//cout<<read<int>(Entity + OFFSET_GLOW_ENABLE) << endl;
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 1); // glow through walls: 2 = enabled, 5 = disabled
		write<int>(Entity + OFFSET_GLOW_T1, 16256);
		write<int>(Entity + OFFSET_GLOW_T2, 1193322764);
		if (Offset::lowHealth)
			write<GlowMode>(Entity + GLOW_TYPE, { 117,117,46,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel; --117，闪烁
		else
			write<GlowMode>(Entity + GLOW_TYPE, { 101,101,46,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + GLOW_DISTANCE, 1200.0f * METER_TO_FLOAT);//玩家发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, color[0] * 45); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, color[1] * 45);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, color[2] * 45); // b
	}
	else {
		write<int>(Entity + OFFSET_GLOW_ENABLE, 2); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 5); // glow through walls: 2 = enabled, 5 = disabled
		write<int>(Entity + OFFSET_GLOW_T1, 0);
		write<int>(Entity + OFFSET_GLOW_T2, 0);

		write<GlowMode>(Entity + GLOW_TYPE, { 12,12,46,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + GLOW_DISTANCE, 1200.0f * METER_TO_FLOAT);//玩家发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, 0); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, 0);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, 0); // b
	}

}
void teammate_glow_f(DWORD64 Entity)
{
	if (1) {
		//cout<<read<int>(Entity + 0x0444)<<read<int>(Entity + 0x09b8)<<endl;
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 2); // glow through walls: 2 = enabled, 5 = disabled
		write<GlowMode>(Entity + GLOW_TYPE, { (int8_t)101,(int8_t)101,35,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + OFFSET_GLOW_COLOR_R, 5); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, 5);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, 5); // b
		/**/
	}
	//cout << read<int>(Entity + OFFSET_GLOW_ENABLE) << endl;
}
void player_glow_down(DWORD64 Entity, float* color)
{
	if (player_glow) {
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 1); // glow through walls: 2 = enabled, 5 = disabled
		write<int>(Entity + OFFSET_GLOW_T1, 16256);
		write<int>(Entity + OFFSET_GLOW_T2, 1193322764);

		write<GlowMode>(Entity + GLOW_TYPE, { 12,12,45,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + GLOW_DISTANCE, 1200.0f * METER_TO_FLOAT);//玩家发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, color[0] * 45); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, color[1] * 45);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, color[2] * 45); // b
		//printf("0x%p,%f,%f,%f\n",Entity, color[0] * 255, color[1] * 255, color[2] * 255);
	}
	else {
		write<int>(Entity + OFFSET_GLOW_ENABLE, 2); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 5); // glow through walls: 2 = enabled, 5 = disabled
		write<int>(Entity + OFFSET_GLOW_T1, 0);
		write<int>(Entity + OFFSET_GLOW_T2, 0);

		write<GlowMode>(Entity + GLOW_TYPE, { 12,12,46,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + GLOW_DISTANCE, 1200.0f * METER_TO_FLOAT);//玩家发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, 0); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, 0);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, 0); // b
	}
}

void ItemGlowEnableFunc(DWORD64 Entity, float* color)
{
	if (item_glow)
	{
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 2); // glow through walls: 2 = enabled, 5 = disabled
		write<GlowMode>(Entity + GLOW_TYPE, { (int8_t)118,(int8_t)118,40,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel; --推荐用137，罗八被动样式
		write<int>(Entity + OFFSET_GLOW_T1, 16256);
		write<int>(Entity + OFFSET_GLOW_T2, 1193322764);

		write<float>(Entity + GLOW_DISTANCE, 150.0f * METER_TO_FLOAT);//物品发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, color[0] * 20); // r color/brightness
		write<float>(Entity + OFFSET_GLOW_COLOR_G, color[1] * 20);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, color[2] * 20); // b
		//printf("i= %d itemid %d\n", i, itemid);
	}
	else
	{
		write<int>(Entity + OFFSET_GLOW_ENABLE, 0); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 5); // glow through walls: 2 = enabled, 5 = disabled

		write<GlowMode>(Entity + GLOW_TYPE, { 0,0,0,0 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + GLOW_DISTANCE, 100.0f * METER_TO_FLOAT);//玩家发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, 0); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, 0);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, 0); // b
	}
	//cout << write<int>(Entity + OFFSET_GLOW_ENABLE, 1)<<read<int>(Entity + OFFSET_GLOW_ENABLE)<< read<int>(Entity + OFFSET_GLOW_THROUGH_WALLS) << endl;
}
void LobaItemGlowEnableFunc(DWORD64 Entity, float* color)
{
	if (item_glow)
	{
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 2); // glow through walls: 2 = enabled, 5 = disabled
		write<GlowMode>(Entity + GLOW_TYPE, { (int8_t)137,(int8_t)137,40,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel; --推荐用137，罗八被动样式
		write<int>(Entity + OFFSET_GLOW_T1, 16256);
		write<int>(Entity + OFFSET_GLOW_T2, 1193322764);

		write<float>(Entity + GLOW_DISTANCE, 150.0f * METER_TO_FLOAT);//物品发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, color[0]); // r color/brightness
		write<float>(Entity + OFFSET_GLOW_COLOR_G, color[1]);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, color[2]); // b
		//printf("i= %d itemid %d\n", i, itemid);
	}
	else
	{
		write<int>(Entity + OFFSET_GLOW_ENABLE, 0); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 5); // glow through walls: 2 = enabled, 5 = disabled

		write<GlowMode>(Entity + GLOW_TYPE, { 0,0,0,0 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + GLOW_DISTANCE, 100.0f * METER_TO_FLOAT);//玩家发光距离
		write<float>(Entity + OFFSET_GLOW_COLOR_R, 0); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, 0);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, 0); // b
	}
	//cout << write<int>(Entity + OFFSET_GLOW_ENABLE, 1)<<read<int>(Entity + OFFSET_GLOW_ENABLE)<< read<int>(Entity + OFFSET_GLOW_THROUGH_WALLS) << endl;
}



float* Set_Color(int shield_type, int shield_health, int health, int teamid, float*& set_color) {
	float* color = 0;

	/**/
	if (Offset::freedm) {
		switch (teamid % 4)
		{
		case 0:
			color = RED; break;
		case 1:
			color = PURPLE; break;
		case 2:
			color = BLUE; break;
		case 3:
			color = GOLD; break;
		}
	}
	else if (show_shield) {
		if (shield_health == 0) {
			color = WHITE;
		}
		else switch (shield_type)
		{
		case 0:
			color = WHITE; break;
		case 1:
			color = WHITE; break;
		case 2:
			color = BLUE;  break;
		case 3:
			color = PURPLE; break;
		case 4:
			color = GOLD; break;
		case 5:
			color = RED; break;
		default:
			color = WHITE; break;
		}
	}
	else
	{
		color = playerglow3;
	}
	set_color = color;
	return color;
}

void PlayerGlowThread()
{
	int shield_type = 100, shield = 50, health = 100,down=0;
	while (QR)
	{

		//write<int>(Offset::localEntity + 0x45c4, 1);
		//cout << read<int>(Offset::localEntity + 0x45c4);
		//player_glow_f(Offset::localEntity, playerglow1);
		//cout<< Offset::players.size()<<read<int>(Offset::localEntity + OFFSET_GLOW_ENABLE) << endl;
		/*if (!Offset::localEntity) continue;
			auto playerList_Copy = Offset::playerList;
		/**/
		/*auto WeaponEntity = GetWeaponEntity(Offset::localEntity);
		cout << read<int>(WeaponEntity+ 0x1804)<<"\t";
		cout << read<float>(WeaponEntity + OFFSET_BULLET_SPEED)<<"\t" << read<float>(WeaponEntity + OFFSET_BULLET_GRAVITY) << endl;
		/**/
		std::string gamemode = GetGamemode(oBaseAddress);
		//cout << gamemode << endl;
		CheckGameMode(gamemode);
		//write <int>(oBaseAddress + 0x01fb0680 + 0x6c, 1);
		if (!Offset::localEntity) continue;
		std::vector<player> playerList_Copy = Offset::players;
		uintptr_t locPlayer = read<uintptr_t>(oBaseAddress + OFFSET_LOCAL_ENT);

		float* color = WHITE;
		int localTeamID = Offset::localTeamID;
		if (0) 
		{
			for (int i = 0; i < 3000; i++) {
				DWORD64 Entity = GetEntityById(i, oBaseAddress);
				std::string signifer = Util::GetSignifier(Entity);
				if (signifer == "")continue;
				//cout << signifer << endl;
				if (signifer == ("player"))
				{
					//player_glow_f(Entity, playerglow1);
					if (player_glow) {
						player_glow_f(Entity, playerglow1);
						cout << Entity<<"\t";
					}
					//cout << read<int>(Entity + OFFSET_GLOW_ENABLE) << endl;
					continue;
				}
				if (signifer == ("npc_dummie"))
				{
					//player_glow_f(Entity, playerglow1);
					if (player_glow) {
						//player_glow_f(Entity, playerglow1);
					}
					//cout << read<int>(Entity + OFFSET_GLOW_ENABLE) << endl;
					continue;
				}
			}
		}
		//if(0)
		//cout << "size:" << playerList_Copy.size();
		for (int i = 0; i < playerList_Copy.size(); i++)
		{
			
			//auto playerEntity = Entity;
			/**/
			auto playerEntity = playerList_Copy[i].Entity;
			uintptr_t Entity = playerEntity;
			if (!Entity) continue;
			entNewVisTime = read<float>(Entity + OFFSET_VISIBLE_TIME);

			// Get entity knocked state
			//playerList_Copy[i].down = IsDown(Entity);
			down = IsDown(Entity);

			// Get entity team ID
			int playerTeamID = GetTeam(Entity);
			//int playerTeamID = playerList_Copy[i].teamid;
			
			//Get shield type
			//playerList_Copy[i].shield_type = GetShieldType(Entity);
			//int shield_type = playerList_Copy[i].shield_type;
			shield_type = GetShieldType(Entity);
			//playerList_Copy[i].shield = read<int>(Entity + OFFSET_SHIELD);
			shield = read<int>(Entity + OFFSET_SHIELD);
			//playerList_Copy[i].health = GetHealth(Entity);
			//playerList_Copy[i].health = read<int>(Entity + OFFSET_HEALTH);
			health = read<int>(Entity + OFFSET_HEALTH);
			/**/

			// Is it an enemy
			/*
			if (Offset::control) {
				if (playerTeamID % 2) playerTeamID = 2;
				else playerTeamID = 1;
				if (localTeamID % 2) localTeamID = 2;
				else localTeamID = 1;
			}/**/
			//if(1)
			if (localTeamID != playerTeamID)
			{
				/*float r = read<float>(Entity + OFFSET_ITEM_COLOR_R);
				float g = read<float>(Entity + OFFSET_ITEM_COLOR_G);
				float b = read<float>(Entity + OFFSET_ITEM_COLOR_B);
				//printf("%f\t%f\t%f\n", r, g, b);
				/**/
				if (50 >= (shield + health))
					Offset::lowHealth = true;
				else
					Offset::lowHealth = false;
				// Is visible
				if (entNewVisTime != entOldVisTime[i])
				//if (entNewVisTime >= entOldVisTime[i] && entNewVisTime > 0.f)
				{
					Offset::players[i].vis = 1;
					//visible enemie color
					if (down == 0)
					{
						//player_glow_f(Entity, GREEN);
						player_glow_f(Entity, playerglow1);
					}
					else
					{
						player_glow_down(Entity, playerglow2);
					}

					entOldVisTime[i] = entNewVisTime;
				}
				else
				{
					//if (visCooldownTime[i] <= 0)
					{
						Offset::players[i].vis = 0;
						//unvisible enemie color
						if (down == 0)
						{
							Set_Color(shield_type, shield, health, playerTeamID, color);
							player_glow_f(Entity, color);

						}
						else
						{
							//unvisible enemie down color
							player_glow_down(Entity, playerglow4);
						}
					}
				}
			}
			else 
			{
				teammate_glow_f(Entity);
			}
			//cout << read<int>(Entity + OFFSET_GLOW_ENABLE) << endl;
		}

		playerList_Copy.clear();

		this_thread::sleep_for(chrono::milliseconds(200));
	}
}
void ItemGlowThread() {
	string itemModelName = { 0 };
	while (QR)
	{
		//PlayerGlowThread();
		if (1) {
			for (int i = 0; i <= 20000; i++) {
				DWORD64 Entity = GetEntityById(i, oBaseAddress);
				//std::string signifer = Util::GetSignifier(Entity);
				//if (signifer != "prop_survival") continue;
				/*if (signifer == ("player"))
				{
					player_glow_f(Entity, playerglow1);
					cout << read<int>(Entity + OFFSET_GLOW_ENABLE) << endl;
					continue;
				}
				/**/
				int itemid = read<int>(Entity + OFFSET_ITEM_ID);
				if (itemid < 1 || itemid > 253)	continue;
				
				float r = read<float>(Entity + OFFSET_ITEM_COLOR_R);
				float g = read<float>(Entity + OFFSET_ITEM_COLOR_G);
				float b = read<float>(Entity + OFFSET_ITEM_COLOR_B);
				float col[] = { r,g,b };
				if (ColCheck(col, 红色物品)
					|| ColCheck(col, 紫色物品)
					|| ColCheck(col, 金色物品))
					LobaItemGlowEnableFunc(Entity,col);
					/**/
				uintptr_t itemModelName_ptr = read<uintptr_t>(Entity + OFFSET_MODEL_NAME);
				itemModelName = readStr(itemModelName_ptr, 80);
				/*
				if (itemid !=173) continue;

				printf("%d\t%s\n", itemid, Utf8ToGb2312(itemModelName.c_str()));
				/**/
				//if (0)
				if (itemid == 234) {
					ItemGlowEnableFunc(Entity, gold_item_col);
				}
				//涡轮
				else if (
					(itemid == 178 && !strcmp(护甲, itemModelName.c_str()))		//红甲
					|| !strcmp(克莱伯, itemModelName.c_str())					//克莱伯
					|| !strcmp(敖犬, itemModelName.c_str())						//敖犬
					|| !strcmp(波塞克, itemModelName.c_str())					//波塞克
					|| !strcmp(暴走, itemModelName.c_str())						//暴走/**/
					) {
					ItemGlowEnableFunc(Entity, red_item_col);
				}
				//if (1);
				//空投枪和红甲
				else if (!strcmp(隔热板, itemModelName.c_str()) || !strcmp(大电, itemModelName.c_str())) {
					//|| itemid == 255 || itemid == 187) {
					if (1)
					{
						ItemGlowEnableFunc(Entity, blue_item_col);//隔热板
					}
					else if (1) {
						ItemGlowEnableFunc(Entity, blue_item_col);//大电
					}
					/*else if (itemid == 255) {
						ItemGlowEnableFunc(Entity, blue_item_col);//重生信标
					}
					else if (itemid == 189)
					{
						ItemGlowEnableFunc(Entity, blue_item_col);//蓝包
					}/**/
				}
				//蓝色物资
				else if (itemid == 168 || itemid == 177 || itemid == 182 || itemid == 186 ||
					itemid == 169 || itemid == 173 || itemid == 183 || itemid == 187) {
					if (itemid == 168 || itemid == 177 || itemid == 182 || itemid == 186)
					{
						ItemGlowEnableFunc(Entity, purple_item_col);//紫装备
					}
					else //if (itemid == 169 || itemid == 173 || itemid == 183 || itemid == 187)//头甲盾包
					{
						ItemGlowEnableFunc(Entity, gold_item_col);//金装备
					}
				}
				//紫金装备
				else if (itemid == 204	//枪管
					|| itemid == 208	//激光瞄准器
					|| itemid == 211	//轻型
					|| itemid == 215	//重型
					|| itemid == 219	//能量
					|| itemid == 223	//狙击
					|| itemid == 227	//枪栓
					|| itemid == 230	//枪托
					|| itemid == 233	//狙枪托
					//紫配件
					|| itemid == 212	//轻型 
					|| itemid == 216	//重型
					|| itemid == 220	//能量 
					|| itemid == 224	//狙击
					|| !strcmp(金十倍, itemModelName.c_str())	//4x-10x金镜
					) {//金配件
					if (itemid == 204	//枪管
						|| itemid == 208	//激光瞄准器
						|| itemid == 211	//轻型
						|| itemid == 215	//重型
						|| itemid == 219	//能量
						|| itemid == 223	//狙击
						|| itemid == 227	//枪栓
						|| itemid == 230	//枪托
						|| itemid == 233) {//狙枪托
						ItemGlowEnableFunc(Entity, purple_item_col);//紫配件
					}
					else /*if (itemid == 212	//轻型
						|| itemid == 216	//重型
						|| itemid == 220	//能量
						|| itemid == 224	//狙击
						|| !strcmp(金十倍, itemModelName.c_str()))		//4x-10x金镜
						/**/
					{
						ItemGlowEnableFunc(Entity, gold_item_col);//金配件
					}
				}
				//紫金配件
					/*else if ((itemid == 6 || itemid == 11 || itemid == 21 || itemid == 26 || itemid == 31 || itemid == 36 || itemid == 46
						|| itemid == 51 || itemid == 56 || itemid == 62 || itemid == 67 || itemid == 73 || itemid == 78 || itemid == 83 || itemid == 88 || itemid == 94
						|| itemid == 99 || itemid == 104 || itemid == 109 || itemid == 114 || itemid == 119 || itemid == 131 || itemid == 136))
					{
						ItemGlowEnableFunc(Entity, gold_item_col);//金枪
					}
				//金枪
				/**/
			}
			do
				this_thread::sleep_for(chrono::milliseconds(1500));
			while (!item_glow);
		}
	}
}
