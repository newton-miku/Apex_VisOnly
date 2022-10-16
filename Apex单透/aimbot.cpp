#define _AIMBOT_CPP
#include "function.h"
#include "offset.h"
#include "util.h"
using namespace std;
Vector3 yupan(const uint64_t& localentity, const uint64_t& playentity, Vector3 target) {
    uintptr_t WeaponEntity = GetWeaponEntity(localentity);
    Vector3 ourself = read<Vector3>(localentity + Offset::camera_origin);
    Vector3 taget_V = read<Vector3>(playentity + OFFSET_ENTITY_VELOCITY);
    Vector3 ourself_V = read<Vector3>(localentity + OFFSET_ENTITY_VELOCITY);
    float juli = sqrt(pow(ourself.x - target.x, 2) + pow(ourself.y - target.y, 2) + pow(ourself.z - target.z, 2));
    float zidan_V = read<float>(WeaponEntity + OFFSET_BULLET_SPEED);
    float zidan_g = read<float>(WeaponEntity + OFFSET_BULLET_GRAVITY);
    Vector3 new_xyz;
    double fly_time;
    if (zidan_V == 1)
    {
        new_xyz.x = target.x;
        new_xyz.y = target.y;
        new_xyz.z = target.z;
        return new_xyz;
    }
    if (zidan_V == 10000)
        //zidan_V = zidan_V + pow(read<int>(WeaponEntity + Offset::m_lastChargeLevel),5.468438819581);
        ;
    fly_time = juli / zidan_V;
    new_xyz.x = target.x + (taget_V.x - ourself_V.x) * fly_time;
    new_xyz.y = target.y + (taget_V.y - ourself_V.y) * fly_time;
    new_xyz.z = target.z + (taget_V.z - ourself_V.z) * fly_time + 750/zidan_g * fly_time * fly_time;
    return new_xyz;
}/**/
namespace Aimbot
{
    void move_to(float x, float y)
    {
        float center_x = ScreenWidth / 2;
        float center_y = ScreenHeight / 2;

        float smooth = Config::AimbotSmooth;

        float target_x = 0.f;
        float target_y = 0.f;

        if (x != 0.f)
        {
            if (x > center_x)
            {
                target_x = -(center_x - x);
                target_x /= smooth;
                if (target_x + center_x > center_x * 2.f) target_x = 0.f;
            }

            if (x < center_x)
            {
                target_x = x - center_x;
                target_x /= smooth;
                if (target_x + center_x < 0.f) target_x = 0.f;
            }
        }

        if (y != 0.f)
        {
            if (y > center_y)
            {
                target_y = -(center_y - y);
                target_y /= smooth;
                target_y /= 10;
                if (target_y + center_y > center_y * 2.f) target_y = 0.f;
            }

            if (y < center_y)
            {
                target_y = y - center_y;
                target_y /= smooth;
                target_y /= 10;
                if (target_y + center_y < 0.f) target_y = 0.f;
            }
        }

        //ÄÚ´æÑ¹Ç¹
        /*
        Vector3 RecoilVec = Util::GetRecoil(Offset::localEntity);
        if (RecoilVec.x != 0 || RecoilVec.y != 0)
        {
            target_x += RecoilVec.x;
            target_y += RecoilVec.y;
        }
        /**/
        
        if ((GetAsyncKeyState(Config::AimbotKey_Code[Config::AimbotKey]) || GetKeyState(VK_CAPITAL))&&!farGoal)
        {
            
                INPUT inputs[1] = {};
                ZeroMemory(inputs, sizeof(inputs));

                inputs[0].type = INPUT_MOUSE;
                inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;
                inputs[0].mi.dx = target_x;
                inputs[0].mi.dy = target_y;

                int nSize = ARRAYSIZE(inputs);
                UINT uSent;
                uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
                /**/
            //mouse_event(MOUSEEVENTF_MOVE, target_x, target_y,0,0);
        }

    }

    void Run()
    {
		Config::EnableAimbot = true;
        while (true)
        {
            bool isAimbotActive = Config::EnableAimbot;
                //(GetAsyncKeyState(Config::AimbotKey_Code[Config::AimbotKey])|| GetKeyState(VK_CAPITAL));
            if (isAimbotActive)
            {
                float target_dist = FLT_MAX;
                DWORD_PTR target_entity = {};

                auto playerList_Copy = Offset::players;
                for (int i = 0; i < playerList_Copy.size(); i++)
                {
                    if (playerList_Copy[i].teamid == Offset::localTeamID) continue;
                    auto playerEntity = playerList_Copy[i].Entity;
                    if (!playerEntity) continue;
					Vector3 local_origin = read<Vector3>(Offset::localEntity + OFFSET_ORIGIN);
					Vector3 player_origin = read<Vector3>(playerEntity + OFFSET_ORIGIN);
					if(!GetKeyState(VK_SCROLL))
					if ((Vector3::distance(local_origin, player_origin)/ 34) > 70) continue;
                    //ÅÐ¶ÏÊÇ·ñ±»»÷µ¹
                    int bleedoutState = read<int>(playerEntity + OFFSET_BLEED_OUT_STATE);
                    if (Config::AimbotIgnoreDowned && bleedoutState > 0) continue;

                    bool Visible = Util::IsVisible(playerEntity, i);
                    if (!Visible) continue;

                    auto aim_pos = Util::WorldToScreen(Util::GetEntityBonePosition(playerEntity, Config::AimbotPosition_Code[Config::AimbotPosition]));

                    float x = aim_pos.x - ScreenWidth / 2.0f;
                    float y = aim_pos.y - ScreenHeight / 2.0f;

                    float crosshair_dist = sqrtf((x * x) + (y * y));

                    if (crosshair_dist <= FLT_MAX && crosshair_dist <= target_dist)
                    {
                        if (crosshair_dist > Config::AimbotFOV) // ·¶Î§
                            continue;

                        target_dist = crosshair_dist;
                        target_entity = playerEntity;
                    }
                }
                playerList_Copy.clear();

                if (target_entity)
                {
                    Vector3 pos = Util::GetEntityBonePosition(target_entity, Config::AimbotPosition_Code[Config::AimbotPosition]);
                    auto head_pos = Util::WorldToScreen(pos);
                    Vector3 newPos = yupan(Offset::localEntity, target_entity, pos);
                    auto new_pos = Util::WorldToScreen(newPos);
                    //if (!GetKeyState(VK_SCROLL) || !GetAsyncKeyState(VK_XBUTTON2))
                    if (!GetKeyState(VK_SCROLL))
                    move_to(head_pos.x, head_pos.y);
                    else
                    move_to(new_pos.x, new_pos.y);
                }
            }
            if (!GetKeyState(VK_NUMLOCK)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(Config::sleeptime));
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
    void Run2()
    {
		Config::EnableAimbot = true;
        while (true)
        {
            bool isAimbotActive = Config::EnableAimbot;
                //(GetAsyncKeyState(Config::AimbotKey_Code[Config::AimbotKey])|| GetKeyState(VK_CAPITAL));
            if (isAimbotActive)
            {
                float target_dist = FLT_MAX;
                DWORD_PTR target_entity = {};


                {
                    auto playerEntity = trackEntity;
                    if (!playerEntity) continue;
                    if (read<int>(playerEntity + OFFSET_TEAM) == Offset::localTeamID) continue;
					Vector3 local_origin = read<Vector3>(Offset::localEntity + OFFSET_ORIGIN);
					Vector3 player_origin = read<Vector3>(playerEntity + OFFSET_ORIGIN);
					if(!GetKeyState(VK_SCROLL))
					if ((Vector3::distance(local_origin, player_origin)/ 34) > 50) continue;
                    //ÅÐ¶ÏÊÇ·ñ±»»÷µ¹
                    int bleedoutState = read<int>(playerEntity + OFFSET_BLEED_OUT_STATE);
                    if (Config::AimbotIgnoreDowned && bleedoutState > 0) continue;
                    int i = trackI;
                    bool Visible = Util::IsVisible(playerEntity, i);
                    if (!Visible) continue;

                    auto aim_pos = Util::WorldToScreen(Util::GetEntityBonePosition(playerEntity, Config::AimbotPosition_Code[Config::AimbotPosition]));

                    float x = aim_pos.x - ScreenWidth / 2.0f;
                    float y = aim_pos.y - ScreenHeight / 2.0f;

                    float crosshair_dist = sqrtf((x * x) + (y * y));

                    if (crosshair_dist <= FLT_MAX && crosshair_dist <= target_dist)
                    {
                        if (crosshair_dist > Config::AimbotFOV) // ·¶Î§
                            continue;

                        target_dist = crosshair_dist;
                        target_entity = playerEntity;
                    }
                }

                if (target_entity)
                {
                    Vector3 pos = Util::GetEntityBonePosition(target_entity, Config::AimbotPosition_Code[Config::AimbotPosition]);
                    auto head_pos = Util::WorldToScreen(pos);
                    Vector3 newPos = yupan(Offset::localEntity, target_entity, pos);
                    auto new_pos = Util::WorldToScreen(newPos);
                    //if (!GetKeyState(VK_SCROLL) || !GetAsyncKeyState(VK_XBUTTON2))
                    if (!GetKeyState(VK_SCROLL))
                    move_to(head_pos.x, head_pos.y);
                    else
                    move_to(new_pos.x, new_pos.y);
                }
            }
            if (!GetKeyState(VK_NUMLOCK)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(Config::sleeptime));
            }
        }
    }
}
