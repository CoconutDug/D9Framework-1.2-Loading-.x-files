#include "windows.h"
#include "Timer.h"
#pragma comment(lib, "Winmm.lib")

Timer::Timer()
{

}

Timer::~Timer()
{

}

unsigned __int64 Timer::GetFrequency() const
{
	unsigned __int64 performanceFrequency = 0;
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&performanceFrequency))
	{
		performanceFrequency = 1000;
	}

	return performanceFrequency;
}

unsigned __int64 Timer::GetTime() const
{
	__int64 timeTicks = 0;

	if (!QueryPerformanceCounter((LARGE_INTEGER *)&timeTicks))
	{
		timeTicks = (__int64)timeGetTime();
	}

	return timeTicks;
}