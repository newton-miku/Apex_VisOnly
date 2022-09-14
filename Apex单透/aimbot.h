#pragma once

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
        
        if ((GetAsyncKeyState(Config::AimbotKey_Code[Config::AimbotKey]) || GetKeyState(VK_CAPITAL)))
        {
            
                INPUT inputs[1] = {};
                ZeroMemory(inputs, sizeof(inputs));

                inputs[0].type = INPUT_MOUSE;
                inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;
                inputs[0].mi.dx = target_x;
                //inputs[0].mi.dy = target_y;

                int nSize = ARRAYSIZE(inputs);
                UINT uSent;
                uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
                /**/
            //mouse_event(MOUSEEVENTF_MOVE, target_x, target_y,0,0);
        }

    }

    void Run()
    {
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

                    //ÅÐ¶ÏÊÇ·ñ±»»÷µ¹
                    int bleedoutState = read<int>(playerEntity + Offset::m_bleedoutState);
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
                    auto head_pos = Util::WorldToScreen(Util::GetEntityBonePosition(target_entity, Config::AimbotPosition_Code[Config::AimbotPosition]));

                    move_to(head_pos.x, head_pos.y);
                }
            }
            if (!GetKeyState(VK_NUMLOCK)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(Config::sleeptime));
            }
        }
    }
}
