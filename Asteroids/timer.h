#pragma once
#include <windows.h>

class Timer
{
private:
	LARGE_INTEGER start;
    LARGE_INTEGER stop;
    LARGE_INTEGER frequency;

public:    
	Timer()
	{
		QueryPerformanceFrequency( &frequency );
	}

    void Start()
	{
		QueryPerformanceCounter( &start ) ;
	}

    void Stop()
	{
		QueryPerformanceCounter( &stop ) ;
	}

    double ElapsedTime()
	{
		return (double)(stop.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
	}
};