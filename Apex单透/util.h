#include <TlHelp32.h>
#include "Driver/MemOperations.h"
namespace Util
{
	//取随机数
	std::string RandomString(int len)
	{
		//srand(time(NULL));
		std::string str = ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		std::string newstr;
		int pos;
		while (newstr.size() != len)
		{
			pos = ((rand() % (str.size() - 1)));
			newstr += str.substr(pos, 1);
		}
		return newstr;
	}


	//重新命名文件
	bool RenameFile(std::string& path)
	{
		std::string newPath = (RandomString(16) + (".exe"));
		SetConsoleTitleA(RandomString(16).c_str());
		if (std::rename(path.c_str(), newPath.c_str())) return false;

		path = newPath;

		return true;
	}

	//根据进程名获取进程ID
	uint64_t GetProcessId(const char* process_name)
	{
		UINT pid = 0;
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 process;
		ZeroMemory(&process, sizeof(process));
		process.dwSize = sizeof(process);
		if (Process32First(snapshot, &process))
		{
			do
			{
				if (std::strcmp(process.szExeFile, process_name) == 0)
				{
					//printf("PID::: %d : [%s] , %s\n\n", process.th32ProcessID, process.szExeFile, process_name);
					pid = process.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &process));
		}
		CloseHandle(snapshot);
		return pid;
	}

	bool IsNullVector2(Vector2 vector)
	{
		if (vector == Vector2{ 0, 0 })
			return true;
		return false;
	}

	Vector3 GetRecoil(DWORD_PTR localEntity)
	{
		return read<Vector3>(localEntity + Offset::m_vecPunchWeapon_Angle);
	}

	Vector3 GetViewAngles(DWORD_PTR entity)
	{
		return read<Vector3>(entity + Offset::m_Viewangles);
	}

	void SetViewAngles(DWORD_PTR entity, Vector3 angles)
	{
		write<Vector3>(entity + Offset::m_Viewangles, angles);
	}

	float lastVisibleTimeList[64];
	bool IsVisible(DWORD_PTR entity, int i)
	{
		float visibleTime = read<float>(entity + Offset::lastVisibleTime);

		bool is_visible = visibleTime > lastVisibleTimeList[i] && visibleTime > 0.f;

		lastVisibleTimeList[i] = visibleTime;

		return is_visible;
	}

	Vector3 GetEntityBasePosition(DWORD_PTR entity)
	{
		return read<Vector3>(entity + Offset::m_vecAbsOrigin);
	}

	Vector3 GetEntityBonePosition(DWORD_PTR entity, int id)
	{
		Vector3 BasePosition = GetEntityBasePosition(entity);
		DWORD_PTR Bone = read<DWORD_PTR>(entity + Offset::m_bones);
		Vector3 bonePosition = {};

		bonePosition.x = read<float>(Bone + 0xCC + (id * 0x30)) + BasePosition.x;
		bonePosition.y = read<float>(Bone + 0xDC + (id * 0x30)) + BasePosition.y;
		bonePosition.z = read<float>(Bone + 0xEC + (id * 0x30)) + BasePosition.z;

		return bonePosition;
	}

	std::string GetSignifier(DWORD_PTR entity)
	{
		uintptr_t SignifierAddr = read<uintptr_t>(entity + Offset::m_iSignifierName);

		char buf[64] = { 0 };
		readmem(SignifierAddr, (uint64_t)&buf, 64);

		return buf;
	}

	D3DMATRIX getViewMatrix()
	{
		DWORD_PTR viewRenderer = read<DWORD_PTR>(dwProcess_Base + Offset::ViewRender);
		if (!viewRenderer) return D3DMATRIX{};

		DWORD_PTR viewMatrix = read<DWORD_PTR>(viewRenderer + Offset::ViewMatrix);
		if (!viewMatrix) return D3DMATRIX{};

		D3DMATRIX m = read<D3DMATRIX>(viewMatrix);

		return m;
	}

	Vector2 WorldToScreen(Vector3 target)
	{
		Vector2 _worldToScreenPos;
		Vector3 to;
		float w = 0.0f;
		D3DMATRIX viewmatrix = getViewMatrix();
		to.x = viewmatrix._11 * target.x + viewmatrix._12 * target.y + viewmatrix._13 * target.z + viewmatrix._14;
		to.y = viewmatrix._21 * target.x + viewmatrix._22 * target.y + viewmatrix._23 * target.z + viewmatrix._24;

		w = viewmatrix._41 * target.x + viewmatrix._42 * target.y + viewmatrix._43 * target.z + viewmatrix._44;

		if (w < 0.01f)
			return Vector2{ 0, 0 };

		to.x *= (1.0f / w);
		to.y *= (1.0f / w);

		int width = ScreenWidth;
		int height = ScreenHeight;

		float x = width / 2;
		float y = height / 2;

		x += 0.5f * to.x * width + 0.5f;
		y -= 0.5f * to.y * height + 0.5f;

		to.x = x;
		to.y = y;

		_worldToScreenPos.x = to.x;
		_worldToScreenPos.y = to.y;
		return _worldToScreenPos;
	}
}