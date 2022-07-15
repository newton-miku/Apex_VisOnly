#pragma once
#include<vector>
//UPDATED 15/07/2022 (DD/MM/YYYY)

#define OFFSET_KEY_JUMP				0x07546398						//in_jump
#define OFFSET_KEY_DUCK				0x0ba314f8						//in_duck

#define OFFSET_ENTITYLIST 			0x1a203b8						//cl_entitylist
#define OFFSET_LOCAL_ENT 			0x1dd15e8						//local_player
#define OFFSET_ORIGIN				0x014c							//BaseEntity!m_vecAbsOrigin
#define OFFSET_BONES				0x0f38							//m_bConstrainBetweenEndpoints //勿动，不会变化
#define OFFSET_NAME_LIST			0xba30ab0						//NameList
#define OFFSET_NAME					0x0589							//m_iName
#define OFFSET_SIG_NAME				0x0580							//m_iSignifierName //Useless
#define OFFSET_GAMEMODE				0x01e09190+0x58					//mp_gamemode + 0x58

#define OFFSET_MATRIX				0x11a210						//view_matrix ViewMatrix
#define OFFSET_RENDER				0x7546250						//view_render ViewRender
#define OFFSET_RadarVIEWANGLES		0x1f4c							//CPlayer!camera_angles
#define OFFSET_HEALTH				0x0438							//CPlayer->m_iHealth
#define OFFSET_SHIELD				0x0170							//CBaseEntity->m_shieldHealth
#define OFFSET_SHIELD_MAX			0x0174							//CBaseEntity->m_shieldHealthMax
#define OFFSET_SHIELD_TYPE			0x4604							//DT_Player->m_armorType
#define OFFSET_LIFESTATE			0x0798							//m_lifeState, 1 = dead

#define OFFSET_OBSERVER_MODE		0x34bc							//m_iObserverMode Type-int 0-not 1/5-watching 3-choosing player
#define OFFSET_OBSERVER_TARGET		0x34c8							//m_hObserverTarget Type-int //no more networked
#define OFFSET_yYaw					0x2a90+0x8						//m_animAimYaw+0x8 ? 
#define OFFSET_YAW					0x22a0-0x8						//m_currentFramePlayer.m_ammoPoolCount - 0x8

#define OFFSET_TEAM					0x0448							//m_iTeamNum
#define OFFSET_BLEED_OUT_STATE		0x2720							//m_bleedoutState,>0 = knocked
#define OFFSET_VISIBLE_TIME			0x1ad4							//CPlayer!lastVisibleTime
#define OFFSET_ITEM_ID				0x16a8							//[RecvTable.DT_PropSurvival]-> m_customScriptInt

#define OFFSET_CAMERAPOS			0x1f40							//CPlayer!camera_origin
#define OFFSET_THIRDPERSON1         0x01db34d0 + 0x6c				//thirdperson_mayamode + 0x6c//0-视角跟随人物视角 !0-人物随意转动视角，镜头视角不转动
#define OFFSET_THIRDPERSON	        0x01a02db0 + 0x6c				//thirdperson_override + 0x6c
#define OFFSET_THIRDPERSONCAM	    0x36a8							//m_thirdPersonShoulderView //1开启，0关闭

#define OFFSET_VIEWANGLES			0x2594 - 0x14					//m_ammoPoolCapacity - 0x14
#define OFFSET_BREATH_ANGLES		(OFFSET_VIEWANGLES - 0x10)
#define OFFSET_WEAPON				0x1a6c							//m_latestPrimaryWeapons
#define OFFSET_BULLET_SPEED         0x1f28							//CWeaponX!m_flProjectileSpeed
#define OFFSET_BULLET_GRAVITY		(OFFSET_BULLET_SPEED + 0x8)		
#define OFFSET_BULLET_SCALE			0x1f30							//CWeaponX!m_flProjectileScale
#define OFFSET_AMMO_IN_CLIP			0x16a4							//CWeaponX!m_ammoInClip		Type-int
#define OFFSET_IN_RELOADING         0x16e6							//CWeaponX!m_bInReload		Type-bool

#define OFFSET_TIMESCALE			0x013fa370						//host_timescale

#define GLOW_DISTANCE				0x0380+0x34						//m_highlightServerFadeEndTimes + 52(0x34)
#define GLOW_TYPE					0x02c0							//m_highlightFunctionBits
#define ITEM_GLOW_TYPE				GLOW_TYPE+4						//m_highlightFunctionBits + 4
#define OFFSET_GLOW_ENABLE          0x3c8							//glow_enable m_highlightServerContextID+8 //1 = enabled, 2 = disabled	//勿动，不随游戏更新而更新
#define OFFSET_GLOW_THROUGH_WALLS   OFFSET_GLOW_ENABLE+0x8			//glow_enable +0x8 //2 = enabled, 5 = disabled
#define OFFSET_GLOW_T1              0x262							//16256 = enabled, 0 = disabled 
#define OFFSET_GLOW_T2              0x2dc							//1193322764 = enabled, 0 = disabled 
#define OFFSET_GLOW_COLOR_R			0x1D0							//glow color for Red
#define OFFSET_GLOW_COLOR_G			OFFSET_GLOW_COLOR_R+0x4			//glow color for Green
#define OFFSET_GLOW_COLOR_B			OFFSET_GLOW_COLOR_R+0x8			//glow color for Blue
typedef struct player {
	DWORD_PTR Entity;
	//std::string name;
	//int health;
	//int shield;
	int shield_type;
	int teamid;
	char name[64] = { 0 };
};

typedef struct watcher {
	char name[64] = { 0 };
};

namespace Offset
{
	std::vector<DWORD_PTR> playerList;
	std::vector<watcher> WatchList;
	std::vector<player> players;
	int localTeamID;

	DWORD_PTR localEntity;

	uintptr_t cl_entitylist = OFFSET_ENTITYLIST;//OFFSET_ENTITYLIST
	uintptr_t LocalPlayer = OFFSET_LOCAL_ENT;//OFFSET_LOCAL_ENT
	uintptr_t ViewRender = OFFSET_RENDER;//OFFSET_RENDER
	uintptr_t ViewMatrix = OFFSET_MATRIX;//OFFSET_MATRIX;

	uintptr_t m_vecAbsOrigin = OFFSET_ORIGIN;// OFFSET_ORIGIN;
	uintptr_t m_bones = OFFSET_BONES;// OFFSET_BONES; //m_bConstrainBetweenEndpoints
	uintptr_t lastVisibleTime = OFFSET_VISIBLE_TIME;// OFFSET_VISIBLE_TIME;
	uintptr_t camera_origin = OFFSET_CAMERAPOS;// OFFSET_CAMERAPOS;
	uintptr_t m_vecPunchWeapon_Angle = 0x0040;

	uintptr_t glow_enable = OFFSET_GLOW_ENABLE; //勿动，不随游戏更新而更新
	uintptr_t glow_visible = OFFSET_GLOW_THROUGH_WALLS; //OFFSET_GLOW_THROUGH_WALLS glow_enable+0x8
	uintptr_t glow_type = GLOW_TYPE;//GLOW_TYPE
	uintptr_t glow_color = OFFSET_GLOW_COLOR_R;

	uintptr_t m_Viewangles = OFFSET_VIEWANGLES; //CPlayer->m_ammoPoolCapacity - 0x14   //OFFSET_VIEWANGLES
	uintptr_t m_iSignifierName = OFFSET_SIG_NAME;// OFFSET_SIG_NAME; //CBaseEntity->m_iSignifierName
	uintptr_t m_iName = OFFSET_NAME;// OFFSET_NAME; //CBaseEntity->m_iName
	uintptr_t m_iTeamNum = OFFSET_TEAM;// OFFSET_TEAM; //CBaseEntity->m_iTeamNum   //用于判断是否是队友
	uintptr_t m_lifeState = OFFSET_LIFESTATE;// OFFSET_LIFESTATE; //CPlayer->m_lifeState  //用于判断玩家是否活着
	uintptr_t m_bleedoutState = OFFSET_BLEED_OUT_STATE;// OFFSET_BLEED_OUT_STATE; //CPlayer->m_bleedoutState  //用于判断玩家是被击倒
}
std::vector<watcher> watcherList;