#pragma once

class Timer
{
public:
	Timer();
	~Timer();
	unsigned __int64 GetFrequency() const;
	unsigned __int64 GetTime() const;
};
