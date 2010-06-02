#ifndef TIMER_CLASS_H
#define TIMER_CLASS_H
#include <windows.h>
#include <mmsystem.h>
#include "basedefs.h"

class EXPORT Timer
{
private:
	LONGLONG lastTicks,pausedTicks,timerFrequency;
	double timerScale;
	Logical isHighPerformanceCounter,isLowResTimer;
	Logical paused,started, tickBasedTimer;
public:
	Timer();
	~Timer();
	void init();
	void start();
	void stop();
	void pause();
	void unpause();
	Logical isPaused();
	Logical isStarted();
	LONGLONG getCurrentTick();
	LONGLONG getTicks();
	double getTimerScale();
	void forceLowResTimer();
	void forceTickBasedTimer();
};
#endif
