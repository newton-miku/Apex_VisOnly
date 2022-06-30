#pragma once

namespace Config
{
	bool EnableAimbot = false;
	bool AimbotIgnoreDowned = true;
	int AimbotFOV = 90;
	int AimbotPosition = 1;
	int AimbotPosition_Code[] = { 8, 7, 5, 3 };
	int AimbotKey = 0;
	DWORD AimbotKey_Code[] = { VK_LMENU, VK_MENU, VK_SHIFT, VK_RSHIFT, VK_CONTROL, VK_RCONTROL, VK_LBUTTON, VK_RBUTTON };
	float AimbotSmooth = 4.0f;
}
