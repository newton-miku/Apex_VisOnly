#pragma once
#include"Driver/MemOperations.h"

extern bool show_shield;
extern bool player_glow;

float playerglow1[3] = { 0.0f, 0.837104f, 0.056f};//可见敌人
float playerglow2[3] = { 0.0f, 0.0f, 1.0f};//可见倒地敌人
float playerglow3[3] = { 0.914416f, 0.004525f, 1.0f};//不可见敌人
float playerglow4[3] = { 0.0f, 0.0f, 1.0f};//不可见倒地敌人

float GREEN[3] = { 0.0f, 1.0f, 0.0f };
float RED[3] = { 1.0f, 0.0f, 0.0f };
float BLUE[3] = {0.0f, 0.83529411f, 1.0f};
float PURPLE[3] = {0.70588f, 0.0f, 1.0f};
float ORANGE[3] = {1.0f, 0.6470588f, 0.0f};
float WHITE[3] = { 1.0f, 1.0f, 1.0f };
float GOLD[3] = {1.0f, 0.933333f, 0.0f};

float entNewVisTime = 0;
float entOldVisTime[100];
int visCooldownTime[100];

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

void player_glow_f(DWORD64 Entity, float* color)
{
	if (player_glow) {
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 1); // glow through walls: 2 = enabled, 5 = disabled
		write<int>(Entity + OFFSET_GLOW_T1, 16256); 
		write<int>(Entity + OFFSET_GLOW_T2, 1193322764); 

		write<GlowMode>(Entity + GLOW_TYPE, { 12,12,46,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
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
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 1); // glow through walls: 2 = enabled, 5 = disabled
		write<GlowMode>(Entity + GLOW_TYPE, { (int8_t)128,(int8_t)128,35,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + OFFSET_GLOW_COLOR_R, 5); // r color/brightness of enemies
		write<float>(Entity + OFFSET_GLOW_COLOR_G, 5);  // g
		write<float>(Entity + OFFSET_GLOW_COLOR_B, 5); // b
	}
}
void player_glow_down(DWORD64 Entity, float* color)
{
	if (player_glow) {
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 1); // glow through walls: 2 = enabled, 5 = disabled
		write<int>(Entity + OFFSET_GLOW_T1, 16256);
		write<int>(Entity + OFFSET_GLOW_T2, 1193322764);

		write<GlowMode>(Entity + GLOW_TYPE, { 117,117,35,95 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
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

std::string GetGamemode(uintptr_t modBase) {
	uintptr_t gameModePtr = read<uintptr_t>(modBase + OFFSET_GAMEMODE);
	return  read<std::string>(gameModePtr);
}
std::string GetName(uintptr_t oBaseAddress, uintptr_t Entity, char name[])
{
	uintptr_t nameindex = read<uintptr_t>(Entity + 0x38);
	uintptr_t name_ptr = read<uintptr_t>(oBaseAddress + OFFSET_NAME_LIST + ((nameindex - 1) << 4));
	std::string names;
	names = readStr(name_ptr, 128);
	//printf("%d %d 0x%p 0x%p %s 1\n", sizeof(long long),sizeof(char[64]),nameindex, name_ptr, names.c_str());;
	strcpy(name, names.c_str());
	return names;
}
int GetTeam(DWORD64 entity) {
	return read<int>(entity + OFFSET_TEAM);
}

DWORD64 GetEntityById(int Ent, DWORD_PTR Base)
{
	DWORD_PTR EntityList = Base + OFFSET_ENTITYLIST; //updated
	DWORD_PTR BaseEntity = read<DWORD_PTR>(EntityList);
	if (!BaseEntity)
		return NULL;
	return  read<DWORD_PTR>(EntityList + (Ent << 5));
}

float* Set_Color(int shield_type,int shield_health, float* &set_color) {
	float* color = 0;
	if (show_shield) {
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


void PlayerGlowFunc()
{
	while (true)
	{
			/*if (!Offset::localEntity) continue;
				auto playerList_Copy = Offset::playerList;
			/**/
			//std::string gamemode = GetGamemode(oBaseAddress);
			if (!Offset::localEntity) continue;
			std::vector<player> playerList_Copy = Offset::players;
			uintptr_t locPlayer = read<uintptr_t>(oBaseAddress + OFFSET_LOCAL_ENT);

			float* color = WHITE;
			//int entTeamID = Offset::localTeamID;
			int entTeamID = read<int>(Offset::localEntity + OFFSET_TEAM);
			for (int i = 0; i < playerList_Copy.size(); i++)
			{
				auto playerEntity = playerList_Copy[i].Entity;
				auto Entity = playerEntity;
				if (!playerEntity) continue;
				entNewVisTime = read<float>(playerEntity + OFFSET_VISIBLE_TIME);

				// Get entity knocked state
				int entKnockedState = read<int>(playerEntity + OFFSET_BLEED_OUT_STATE);

				// Get player team ID,Just for the localplayer's teamID
				//int playerTeamID = read<int>(locPlayer + OFFSET_TEAM);

				// Get entity team ID
				//int entTeamID = read<int>(playerEntity + OFFSET_TEAM);
				int playerTeamID = read<int>(playerList_Copy[i].Entity + OFFSET_TEAM);

				//Get shield type
				int shield_type = read<int>(Entity + OFFSET_SHIELD_TYPE);
				int shield_health = read<int>(Entity + OFFSET_SHIELD);


				// Is it an enemy
				/*if (strcmp(gamemode.c_str(), ("control"))) {
					if (playerTeamID % 2) playerTeamID = 2;
					else playerTeamID = 1;
					if (entTeamID % 2) entTeamID = 2;
					else entTeamID = 1;
				}*/
				if (entTeamID != playerTeamID)
				{

					// Is visible
					if (entNewVisTime != entOldVisTime[i])
					{
						visCooldownTime[i] = 0; // low values mean less latency, increase if you observe the color changes on visible enemies

						//visible enemie color
						if (entKnockedState == 0)
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
						if (visCooldownTime[i] <= 0)
						{
							//unvisible enemie color
							if (entKnockedState == 0)
							{
								Set_Color(shield_type, shield_health, color);
								player_glow_f(Entity, color);
							}
							else
							{
								//unvisible enemie down color
								player_glow_down(Entity, playerglow4);
							}
						}
					}

					if (visCooldownTime[i] >= 0) visCooldownTime[i] -= 1;
				}
				else {
					teammate_glow_f(playerEntity);
				}
			}

			playerList_Copy.clear();

			Sleep(200);
		}
}
