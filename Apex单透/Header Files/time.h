#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <winternl.h>

using namespace std;
static DWORD startTime, stopTime;
/*
void init_time() {
	startTime = GetTickCount64();
	stopTime = GetTickCount64();
}
DWORD get_time() {
	DWORD time = GetTickCount64();
	//ms
	return (time-startTime);
}/**/
static LARGE_INTEGER TIME_START;
static LARGE_INTEGER TIME_FREQ;

bool init_time() {
	bool ret;
	ret = QueryPerformanceFrequency(&TIME_FREQ);
	if (ret) {
		ret = QueryPerformanceCounter(&TIME_START);
	}
	else
	{
		init_time();
	}
	return ret;
}
double get_time() {
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	//long long elapsed = (time.QuadPart - TIME_START.QuadPart)*1000000/ (TIME_FREQ.QuadPart);
	double elapsed = static_cast<double>(time.QuadPart - TIME_START.QuadPart) / static_cast<double>(TIME_FREQ.QuadPart);
	//double elapsed = static_cast<double>(time.QuadPart - TIME_START.QuadPart) / static_cast<double>(TIME_FREQ.QuadPart);
	return elapsed;
}