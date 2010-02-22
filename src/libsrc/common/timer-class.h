#ifndef TIMER_CLASS_H
#define TIMER_CLASS_H
#include <windows.h>
#include <mmsystem.h>
#include "basedefs.h"

class EXPORT Timer
{
private:
	LONGLONG startTicks,pausedTicks,timerFrequency;
	double timeCount;
	double timerScale, fps;
	Logical isHighPerformanceCounter,isLowResTimer;
	Logical paused,started;
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
	void setFPS(double fps);
	double getFPS();
	LONGLONG getCurrentTick();
	LONGLONG getTicks();
	double getTimerScale();
	double getTimeCount();
	void forceLowResTimer();
	//void static sayHello();
};
#endif
