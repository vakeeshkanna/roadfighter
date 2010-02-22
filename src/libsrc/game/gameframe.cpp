#include "gameframe.h"

GameFrame::GameFrame()
{
	timer.forceLowResTimer();
	setFPS(120);
	timer.init();
	currentTime = 0;
	nextTime = timer.getCurrentTick();
	lastTime = nextTime;
	timeScale = timer.getTimerScale();
	timeCount = timer.getTimeCount();
}

GameFrame::~GameFrame()
{

}

void GameFrame::setFPS(double frameps)
{
	fps = frameps;
	timer.setFPS(fps);
	timer.init();
}

double GameFrame::getFPS()
{
	return fps;
}

void GameFrame::init()
{
	currentTime = 0;
	nextTime = timer.getCurrentTick();
	lastTime = nextTime;
	timeScale = timer.getTimerScale();
	timeCount = timer.getTimeCount();
}

double GameFrame::getTimeElapsed()
{
	double timeElapsed = 0;
	currentTime = timer.getCurrentTick();
	//lprintf("%ld ", currentTime);
	//lprintf("%ld\n", nextTime);

	if(currentTime > nextTime)
	{
		//lprintf("inside %ld",currentTime - lastTime);
		timeElapsed = (currentTime - lastTime) * timeScale;
		lastTime = currentTime;
		nextTime = currentTime + timeCount;
	}
	//lprintf(" %f", timeScale);
	//lprintf(" %lf\n", timeCount);
	//lprintf("%ld\n", timeElapsed);
	return timeElapsed;
}

