#pragma once
#include <windows.h>

class TimeSpan {
public:
	LARGE_INTEGER start, stop, frequency;

	TimeSpan(LARGE_INTEGER start, LARGE_INTEGER stop, LARGE_INTEGER frequency) {
		this->start = start;
		this->stop = stop;
		this->frequency = frequency;
	}
	
	long Ticks() {
		return stop.QuadPart - start.QuadPart;
	}
	double Seconds() {
		return (stop.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
	}
};

class Time {
public:
	LARGE_INTEGER start, prev, now, frequency;
	Time(LARGE_INTEGER start, LARGE_INTEGER prev, LARGE_INTEGER now, LARGE_INTEGER frequency) {
		this->start = start;
		this->prev = prev;
		this->now = now;
		this->frequency = frequency;
	}

	TimeSpan Elapsed() {
		return TimeSpan(prev, now, frequency);
	}
	TimeSpan Total() {
		return TimeSpan(start, now, frequency);
	}
};

class Timer
{
private:
	LARGE_INTEGER begin, start, now, stop, frequency;

public:    
	Timer()
	{
		QueryPerformanceFrequency( &frequency );
		QueryPerformanceCounter( &begin );
	}

    void Start()
	{
		QueryPerformanceCounter( &start );
	}

    void Stop()
	{
		QueryPerformanceCounter( &stop );
	}

	TimeSpan TotalTime()
	{
		QueryPerformanceCounter( &now );
		return TimeSpan(begin, now, frequency);
	}
	TimeSpan ElapsedTime() 
	{		
		return TimeSpan(start, stop, frequency);
	}
	TimeSpan DeltaTime()
	{
		LARGE_INTEGER prev = now;		
		QueryPerformanceCounter( &now );
		return TimeSpan(prev, now, frequency);
	}
	Time GetTime() {
		LARGE_INTEGER prev = now;		
		QueryPerformanceCounter( &now );
		return Time(begin, prev, now, frequency);
	}
};