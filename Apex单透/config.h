#pragma once

namespace Config
{
	bool EnableAimbot = true;
	bool AimbotIgnoreDowned = true;
	int AimbotFOV = 100;
	int AimbotPosition = 2;
	int AimbotPosition_Code[] = { 8, 7, 5, 3 };
	int AimbotKey = 0;
	DWORD AimbotKey_Code[] = { VK_RMENU, VK_MENU, VK_SHIFT, VK_RSHIFT, VK_CONTROL, VK_RCONTROL, VK_LBUTTON, VK_RBUTTON };
	float AimbotSmooth = 5.0f;
	int sleeptime = 250.0f;
}
