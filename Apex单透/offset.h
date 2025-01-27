#include<vector>
#include <string>
#include "vector.h"
#pragma once
#define _OFFSET_H
//UPDATED 14/10/2022 (DD/MM/YYYY)

#define OFFSET_KEY_JUMP				0x075cc360						//in_jump
#define OFFSET_KEY_DUCK				0x0bacffd8						//in_duck

#define OFFSET_ENTITYLIST 			0x1a9e778						//cl_entitylist
#define OFFSET_LOCAL_ENT 			0x1e4f288						//LocalPlayer local_player
#define OFFSET_ORIGIN				0x014c							//BaseEntity!m_vecAbsOrigin
#define OFFSET_BONES				0x0e88 + 0x48					//m_nForceBone +0x48 //勿动，不会变化
#define OFFSET_NAME_LIST			0xbacf590						//NameList
#define OFFSET_NAME					0x0589							//m_iName
#define OFFSET_SIG_NAME				0x0580							//m_iSignifierName //Useless
#define OFFSET_GAMEMODE				0x01e87f80 + 0x58				//mp_gamemode + 0x58
#define OFFSET_LEVELNAME			0x13c3e38						//LevelName		大厅mp_lobby
#define OFFSET_MODEL_NAME			0x0030							//m_ModelName
#define OFFSET_RENDER				0x75cc210						//ViewRender view_render
#define OFFSET_MATRIX				0x11a210						//ViewMatrix view_matrix

#define OFFSET_RadarVIEWANGLES		0x1eec							//CPlayer!camera_angles
#define OFFSET_HEALTH				0x043c							//CPlayer->m_iHealth
#define OFFSET_SHIELD				0x0170							//CBaseEntity->m_shieldHealth
#define OFFSET_SHIELD_MAX			0x0174							//CBaseEntity->m_shieldHealthMax
#define OFFSET_SHIELD_TYPE			0x45bc							//DT_Player->m_armorType
#define OFFSET_LIFESTATE			0x0798							//m_lifeState, 1 = dead
#define OFFSET_TEAM					0x044c							//m_iTeamNum
#define OFFSET_BLEED_OUT_STATE		0x26d0							//m_bleedoutState,>0 = knocked
#define OFFSET_VISIBLE_TIME			0x1a48							//CPlayer!lastVisibleTime
#define OFFSET_ENTITY_VELOCITY		0x0488							//m_vecVelocity 人物速度

/*
#define OFFSET_OBSERVER_MODE		0x3464							//m_iObserverMode Type-int 0-not 1/5-watching 3-choosing player
#define OFFSET_OBSERVER_TARGET		0x3470							//m_hObserverTarget Type-int //no more networked
/**/
#define OFFSET_YAW					0x2244-0x8						//m_currentFramePlayer.m_ammoPoolCount - 0x8

#define OFFSET_CAMERAPOS			0x1ee0							//CPlayer!camera_origin
#define OFFSET_CAMERAANG			0x1eec							//CPlayer!camera_angles
#define OFFSET_THIRDPERSON1         0x01e2daf0 + 0x6c				//thirdperson_mayamode + 0x6c//0-视角跟随人物视角 !0-人物随意转动视角，镜头视角不转动
#define OFFSET_THIRDPERSON	        0x01a7da30 + 0x6c				//thirdperson_override + 0x6c
#define OFFSET_THIRDPERSONCAM	    0x3650							//m_thirdPersonShoulderView //1开启，0关闭

#define OFFSET_ZOOMING				0x1c01							//m_bZooming
#define OFFSET_ZOOM_FOV				0x1698+0x00b8					//m_playerData + m_curZoomFOV
#define OFFSET_VIEWANGLES			0x253c - 0x14					//m_ammoPoolCapacity - 0x14
#define OFFSET_BREATH_ANGLES		(OFFSET_VIEWANGLES - 0x10)
#define OFFSET_WEAPON				0x19ec							//m_latestPrimaryWeapons
#define OFFSET_BULLET_SPEED         0x1ed0							//CWeaponX!m_flProjectileSpeed
#define OFFSET_BULLET_GRAVITY		(OFFSET_BULLET_SPEED + 0x8)		
#define OFFSET_BULLET_SCALE			0x1ed8							//CWeaponX!m_flProjectileScale
#define OFFSET_AMMO_IN_CLIP			0x1624							//CWeaponX!m_ammoInClip		Type-int
#define OFFSET_IN_RELOADING         0x1666							//CWeaponX!m_bInReload		Type-bool

#define OFFSET_TIMESCALE			0x0143e590						//host_timescale

#define OFFSET_ITEM_ID				0x1628							//[RecvTable.DT_PropSurvival]-> m_customScriptInt
#define GLOW_DISTANCE				0x0380+0x34						//m_highlightServerFadeEndTimes + 52(0x34)
#define GLOW_TYPE					0x02c0 + 0x4					//m_highlightFunctionBits + 4
#define OFFSET_GLOW_ENABLE          0x03c0 + 0x8					//m_highlightServerContextID+8 //1 = enabled, 2 = disabled	//勿动，不随游戏更新而更新
#define OFFSET_GLOW_THROUGH_WALLS   OFFSET_GLOW_ENABLE+0x8			//OFFSET_GLOW_ENABLE +0x8		//2 = enabled, 5 = disabled
#define OFFSET_GLOW_T1              0x262							//16256 = enabled, 0 = disabled 
#define OFFSET_GLOW_T2              0x2dc							//1193322764 = enabled, 0 = disabled 

#define OFFSET_ITEM_COLOR_R			0x01b8							//m_highlightParams				//get default colro for red
#define OFFSET_ITEM_COLOR_G			OFFSET_ITEM_COLOR_R+0x4			//OFFSET_ITEM_COLOR_R+0x4
#define OFFSET_ITEM_COLOR_B			OFFSET_ITEM_COLOR_R+0x8			//OFFSET_ITEM_COLOR_R+0x8
#define OFFSET_GLOW_COLOR_R			0x01b8 + 0x18					//glow color for Red			//m_highlightParams + 24 (0x18)
#define OFFSET_GLOW_COLOR_G			OFFSET_GLOW_COLOR_R+0x4			//glow color for Green
#define OFFSET_GLOW_COLOR_B			OFFSET_GLOW_COLOR_R+0x8			//glow color for Blue

//m_hViewModels 如果想让手中武器发光，请查阅与此相关的资料

#define METER_TO_FLOAT 3000.0f / 70.0f //meter to float unit

inline std::string latestVer, thisVer, updatetime;
inline bool QR = false, debug = false;
inline bool k_f5 = 0;
inline bool k_f6 = 0;
inline bool k_f7 = 0;
inline bool k_f8 = 0;

inline bool k_left = 0;
inline bool k_right = 0;
inline bool k_up = 0;
inline bool k_down = 0;

inline bool player_glow = true;
inline bool show_shield = true;
inline bool farGoal;
inline int	trackI;
uint64_t trackEntity;

inline bool show_watcher = true;
inline bool hide_console = true;
inline bool item_glow = false;

inline float gold_item_col[] = { 0.94f, 1.0f, 0.0f };//金色
inline float red_item_col[] = { 1.0f, 0.0f, 0.0f };//红色
inline float purple_item_col[] = { 0.588235f, 0.0f, 1.0f };//紫色
inline float blue_item_col[] = { 0.2805f, 0.7558f, 1.0f };//蓝色

inline float lastVisibleTimeList[64];

inline float entNewVisTime = 0;
inline float entNewVisTime1 = 0;
inline float entOldVisTime[100];
inline float entOldVisTime1[100];
inline int visCooldownTime[100];

typedef struct player{
	uintptr_t Entity;
	Vector3 origin;
	int vis=0;
	int i;
	//std::string name;
	int dead;//dead == 1，player is dead
	int down;//down == 1，player is down
	int health;
	int shield;
	int shield_type;
	int teamid;
	char name[64] = { 0 };
};

typedef struct watcher{
	int team;
	char name[64] = { 0 };
};

namespace Offset
{
	//std::vector<DWORD_PTR> playerList;
	std::vector<watcher> WatchList;
	std::vector<player> players;
	bool lowHealth = false;

	inline bool freedm = false;
	inline bool control = false;
	inline int localTeamID;
	inline Vector3 localCam;
	inline Vector3 localCamAngles;

	inline uintptr_t localEntity;
	inline uintptr_t m_nameVisibilityFlags = 0x09b8;

	inline uintptr_t cl_entitylist = OFFSET_ENTITYLIST;//OFFSET_ENTITYLIST
	inline uintptr_t LocalPlayer = OFFSET_LOCAL_ENT;//OFFSET_LOCAL_ENT
	inline uintptr_t ViewRender = OFFSET_RENDER;//OFFSET_RENDER
	inline uintptr_t ViewMatrix = OFFSET_MATRIX;//OFFSET_MATRIX;

	inline uintptr_t m_vecAbsOrigin = OFFSET_ORIGIN;// OFFSET_ORIGIN;
	inline uintptr_t m_bones = OFFSET_BONES;// OFFSET_BONES; //m_bConstrainBetweenEndpoints
	inline uintptr_t lastVisibleTime = OFFSET_VISIBLE_TIME;// OFFSET_VISIBLE_TIME;
	inline uintptr_t camera_origin = OFFSET_CAMERAPOS;// OFFSET_CAMERAPOS;
	inline uintptr_t m_vecPunchWeapon_Angle = 0x0040;

	inline uintptr_t glow_enable = OFFSET_GLOW_ENABLE; //勿动，不随游戏更新而更新
	inline uintptr_t glow_visible = OFFSET_GLOW_THROUGH_WALLS; //OFFSET_GLOW_THROUGH_WALLS glow_enable+0x8
	inline uintptr_t glow_type = GLOW_TYPE;//GLOW_TYPE
	inline uintptr_t glow_color = OFFSET_GLOW_COLOR_R;

	inline uintptr_t m_Viewangles = OFFSET_VIEWANGLES; //CPlayer->m_ammoPoolCapacity - 0x14   //OFFSET_VIEWANGLES
	inline uintptr_t m_iSignifierName = OFFSET_SIG_NAME;// OFFSET_SIG_NAME; //CBaseEntity->m_iSignifierName
	inline uintptr_t m_iName = OFFSET_NAME;// OFFSET_NAME; //CBaseEntity->m_iName
	inline uintptr_t m_iTeamNum = OFFSET_TEAM;// OFFSET_TEAM; //CBaseEntity->m_iTeamNum   //用于判断是否是队友
	inline uintptr_t m_lifeState = OFFSET_LIFESTATE;// OFFSET_LIFESTATE; //CPlayer->m_lifeState  //用于判断玩家是否活着
	inline uintptr_t m_bleedoutState = OFFSET_BLEED_OUT_STATE;// OFFSET_BLEED_OUT_STATE; //CPlayer->m_bleedoutState  //用于判断玩家是被击倒
}
std::vector<watcher> watcherList;