#include "timer-class.h"
#include <windows.h>
#include "log-class.h"
#include "basedefs.h"

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	timerScale = 0.0;
	timeCount = 0;
	fps = 120;
	started = no;
	paused = no;
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
		timerScale = 1.0 / timerFrequency;
	}
	else
	{
		isHighPerformanceCounter = no;
		timerScale = 0.001;
	}
	timeCount = (1.0 / timerScale) / fps;
	//lprintf("timerScale = %lf timecount = %lf\n", timerScale, timeCount);
}

void Timer::start()
{
	started = yes;
	paused = no;
	if(isHighPerformanceCounter && !isLowResTimer)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&startTicks);
	}
	else
	{
		startTicks = timeGetTime();
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
		pausedTicks = current - startTicks;
	}
}

void Timer::unpause()
{
	LONGLONG current;
	if(isPaused())
	{
		paused = no;
		startTicks = current - pausedTicks;
		pausedTicks = 0;
	}
}

Logical Timer::isPaused()
{
	return paused;
}

Logical Timer::isStarted()
{
	return started;
}

void Timer::setFPS(double framePerSecond)
{
	fps = framePerSecond;
}

double Timer::getFPS()
{
	return fps;
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
	LONGLONG current;
	if( started == yes )
	{
		if( paused == yes )
		{
			return pausedTicks;
		}
		else
		{
			current = getCurrentTick();
			return current - startTicks;
		}
	}
	return 0;
}

double Timer::getTimerScale()
{
	return timerScale;
}

double Timer::getTimeCount()
{
	return timeCount;
}

void Timer::forceLowResTimer()
{
	isLowResTimer = yes;
}
