#include "framerate.h"

FrameRate* FrameRate::fRate = NULL;

FrameRate::FrameRate()
{
	speedFactor = 0.0;
}

void FrameRate::init(float tfps)
{
  targetFPS = tfps;
  QueryPerformanceCounter(&frameDelay);
  QueryPerformanceFrequency(&ticksPerSecond);
}

void FrameRate::setSpeedFactor()
{
  QueryPerformanceCounter(&currentTicks);

  //This frame's length out of desired length
  speedFactor = (float)(currentTicks.QuadPart - frameDelay.QuadPart)/((float)ticksPerSecond.QuadPart / targetFPS);
  fps = targetFPS / speedFactor;
  if (speedFactor <= 0)
    speedFactor = 1;

  frameDelay = currentTicks;
}

float FrameRate::getSpeedFactor()
{
  return speedFactor;
}

FrameRate* FrameRate::getInstance()
{
	if(fRate == NULL)
	{
		fRate = new FrameRate();
	}
	return fRate;
}

