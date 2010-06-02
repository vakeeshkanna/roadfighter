#include "timer-class.h"
#include <windows.h>
#include "log-class.h"
#include "basedefs.h"

Timer::Timer()
{
	lastTicks = 0;
	pausedTicks = 0;
	timerScale = 0.0;
	started = no;
	paused = no;
	tickBasedTimer = yes;
	isHighPerformanceCounter = no;
	isLowResTimer = no;
}

Timer::~Timer()
{

}

void Timer::init()
{
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&timerFrequency) && !isLowResTimer)
	{
		//lprintf("inside high perf\n");
		isHighPerformanceCounter = yes;
		timerScale = 1.0 / timerFrequency; // 1 microsecond usually
	}
	else
	{
		isHighPerformanceCounter = no;
		timerScale = 1 / 1000.0;  // 1 millisecond
	}
	//lprintf("timerScale = %lf timecount = %lf\n", timerScale, timeCount);
}

void Timer::start()
{
	started = yes;
	paused = no;
	if(isHighPerformanceCounter && !isLowResTimer)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&lastTicks);
	}
	else
	{
		lastTicks = timeGetTime();
	}
}

void Timer::stop()
{
	started = no;
	paused = no;
}

void Timer::pause()
{
	LONGLONG current;
	if(isStarted() && !isPaused())
	{
		paused = yes;
		current = getCurrentTick();
		pausedTicks = current - lastTicks;
	}
}

void Timer::unpause()
{
	LONGLONG current;
	if(isPaused())
	{
		paused = no;
		lastTicks = current - pausedTicks;
		pausedTicks = 0;
	}
}

void Timer::forceTickBasedTimer()
{
	tickBasedTimer = yes;
}

Logical Timer::isPaused()
{
	return paused;
}

Logical Timer::isStarted()
{
	return started;
}

LONGLONG Timer::getCurrentTick()
{
	LONGLONG current;
	if(isHighPerformanceCounter && !isLowResTimer)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&current);
	}
	else
		current = timeGetTime();

	return current;
}

LONGLONG Timer::getTicks()
{
	LONGLONG current, ticks = 0;
	if( started == yes )
	{
		if( paused == yes )
		{
			return pausedTicks;
		}
		else
		{
			current = getCurrentTick();
			ticks = current - lastTicks;
			if(!tickBasedTimer)
			{
				lastTicks = current;
			}
		}
	}
	return ticks;
}

double Timer::getTimerScale()
{
	return timerScale;
}

void Timer::forceLowResTimer()
{
	isLowResTimer = yes;
}
