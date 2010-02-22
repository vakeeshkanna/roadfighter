#ifndef GAME_FRAME_H
#define GAME_FRAME_H

#include "timer-class.h"

class EXPORT GameFrame
{
public:
	GameFrame();
	~GameFrame();
	void init();
	double getTimeElapsed();
	void setFPS(double fps);
	double getFPS();

private:
	Timer timer;
	LONGLONG currentTime, nextTime, lastTime;
	double timeCount;
	double fps, timeScale;
};
#endif
