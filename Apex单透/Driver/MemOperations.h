#pragma once
#include "utils.h"

#define GAME_PROCESS_NAME L"r5apex.exe"
#define TEST_PROCESS_NAME L"EasyAntiCheat_launcher.exe"
#define DRIVER_FILENAMEW "\\\\.\\Newton39G"
//#define DRIVER_FILENAMEW "\\\\.\\Fafaki"
#define IO_READorWRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0391, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_GET_BASE1 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0392, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0393, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_GET_BASE2 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0394, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define DRIVER_NOT_LOADED 1
#define PROCESS_NOT_FOUND 2
DWORD GAME_PROCESS_ID;
DWORD CURRENT_PROCESS_ID;
HANDLE hDriver;

inline uintptr_t oBaseAddress = 0;
inline HWND hwnd = NULL;
inline DWORD oPID;

typedef struct _KERNEL_READorWRITE_REQUEST
{
	uint32_t sourceProcessId;
	uint32_t destProcessId;
	uint64_t sourceAddress;
	uint64_t destAddress;
	uint32_t Size;
} KERNEL_READorWRITE_REQUEST, * PKERNEL_READorWRITE_REQUEST;

typedef struct _KERNEL_GETUNITYPLAYER_REQUEST
{
	uint32_t gameProcessId; //gameprocessId
	uint32_t destProcessId; //usermode hack process id
	uint64_t sourceAddress; // usermode hack buffer
	const char* module_name;
} KERNEL_GETUNITYPLAYER_REQUEST, * PKERNEL_GETUNITYPLAYER_REQUEST;

bool ReadVirtualMemory(HANDLE hDriver, uint32_t sourceProcessId, uint64_t sourceAddress, uint64_t readbuffer, uint32_t Size)
{
	KERNEL_READorWRITE_REQUEST ReadRequest;
	DWORD Bytes;
	ReadRequest.sourceProcessId = sourceProcessId;
	ReadRequest.sourceAddress = sourceAddress;
	ReadRequest.destProcessId = GetCurrentProcessId();
	ReadRequest.destAddress = readbuffer;
	ReadRequest.Size = Size;
	if (DeviceIoControl(hDriver, IO_READorWRITE_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), &Bytes, 0))
		return true;
	else
		return false;
}

uint64_t get_module_base(const char* module_name, uint64_t readbuffer = 0)
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return 0;
	KERNEL_GETUNITYPLAYER_REQUEST req;
	req.gameProcessId = GAME_PROCESS_ID;
	req.sourceAddress = readbuffer;
	req.destProcessId = GetCurrentProcessId();
	req.module_name = module_name;
	DWORD bytes_read;
	printf("基地址方式：");
	if (DeviceIoControl(hDriver, IO_GET_BASE, &req, sizeof(req), &req, sizeof(req), &bytes_read, 0))
	{
		if (!req.sourceAddress) {
			if (DeviceIoControl(hDriver, IO_GET_BASE1, &req, sizeof(req), &req, sizeof(req), &bytes_read, 0)) {
				if (!req.sourceAddress) {
					if (DeviceIoControl(hDriver, IO_GET_BASE2, &req, sizeof(req), &req, sizeof(req), &bytes_read, 0)) {
						printf("2");
						return req.sourceAddress;
					}
					else {
						printf(E("驱动出错\n"));
						return req.sourceAddress;
					}
				}
				else
				{
					printf("1\n");
					return req.sourceAddress;
				}
			}
			else
			{
				printf(E("驱动出错\n"));
				return req.sourceAddress;
			}
		}
		else
		{
			printf("0\n");
			return req.sourceAddress;
		}


	}
	printf(E("驱动出错\n"));
	return req.sourceAddress;
}

int Init()
{
	//hDriver = CreateFileA(DRIVER_FILENAMEW, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	hDriver = CreateFileA(DRIVER_FILENAMEW, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	GAME_PROCESS_ID = GetProcessIdByName(GAME_PROCESS_NAME);
	/*if (GAME_PROCESS_ID == 0) {
		printf("正在查询测试程序\n");
		GAME_PROCESS_ID = GetProcessIdByName(TEST_PROCESS_NAME);
	}*/
	CURRENT_PROCESS_ID = GetCurrentProcessId();
	printf("Game PID:%d\n当前程序 PID：%d\n", GAME_PROCESS_ID, CURRENT_PROCESS_ID);
	if (hDriver == INVALID_HANDLE_VALUE)
		return DRIVER_NOT_LOADED;
	else if (GAME_PROCESS_ID == 0)
		return PROCESS_NOT_FOUND;
	else
		return 0;
}

void readmem(void* addresstoread, void* buffer, uint32_t size)
{
	if ((uint64_t)addresstoread < 0x10000) //address in non paged area
	{
		buffer = 0;
		return;
	}
	ReadVirtualMemory(hDriver, GAME_PROCESS_ID, (uint64_t)addresstoread, (uint64_t)buffer, size);
	//SockRequest((uint64_t)addresstoread, (uint64_t)buffer, size);

}
void readmem(uint64_t dst_addr, uint64_t src_addr, size_t size)
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return;
	KERNEL_READorWRITE_REQUEST request{ GAME_PROCESS_ID,GetCurrentProcessId(), dst_addr, src_addr, size };
	DWORD bytes_read;
	DeviceIoControl(hDriver, IO_READorWRITE_REQUEST, &request, sizeof(KERNEL_READorWRITE_REQUEST), 0, 0, &bytes_read, 0);

}void writemem(uint64_t dst_addr, uint64_t src_addr, size_t size)
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return;
	KERNEL_READorWRITE_REQUEST request;
	request.sourceProcessId = GetCurrentProcessId();
	request.sourceAddress = src_addr;
	request.destProcessId = GAME_PROCESS_ID;
	request.destAddress = dst_addr;
	request.Size = size;
	//KERNEL_READorWRITE_REQUEST request{ GetCurrentProcessId(), GAME_PROCESS_ID, src_addr, dst_addr, size };
	DWORD bytes_read;
	DeviceIoControl(hDriver, IO_READorWRITE_REQUEST, &request, sizeof(KERNEL_READorWRITE_REQUEST), 0, 0, &bytes_read, 0);

}
void readmem(uint64_t addresstoread, void* buffer, uint32_t size)
{
	if (addresstoread < 0x10000) //address in non paged area
	{
		buffer = 0;
		return;
	}

	ReadVirtualMemory(hDriver, GAME_PROCESS_ID, (uint64_t)addresstoread, (uint64_t)buffer, size);
	//SockRequest((uint64_t)addresstoread, (uint64_t)buffer, size);

}

uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets) {
	uint64_t result = 0;
	readmem(base + offsets.at(0), &result, sizeof(uint64_t));
	if (!result)
		return 0;
	for (int i = 1; i < offsets.size(); i++) {
		readmem(result + offsets.at(i), &result, sizeof(uint64_t));
		if (!result)
			return 0;
	}
	return result;
}

template <typename type>
type read(uint64_t src)
{
	type ret = {};
	readmem(src, (uint64_t)&ret, sizeof(type));
	return ret;
}

template <typename type>
type read(uint64_t src, int len)
{
	type ret = {};
	readmem(src, (uint64_t)&ret, len);
	return ret;
}

std::string readStr(uint64_t src, int len)
{
	std::string result;
	char ret = { 0 };
	for (int i = 0; i < len; i++) {
		readmem(src + i, (uint64_t)&ret, sizeof(char));
		result += ret;
	}
	return result;
}

template <typename type>
void write(uint64_t dst_addr, type var)
{
	type inp = var;
	writemem(dst_addr, (uint64_t)&inp, sizeof(type));
}