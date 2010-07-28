#ifndef FRAMERATE
#define FRAMERATE

//Global Handle to FrameRate
#define FR FrameRate::getInstance()

class FrameRate
{
public:
	float  targetFPS;
	float  fps;
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER currentTicks;
	LARGE_INTEGER frameDelay;
	float speedFactor;
	unsigned int timeElapsed;
	static FrameRate* fRate;

	FrameRate();
	void init(float tfps);
	void setSpeedFactor();
	float getSpeedFactor();
	void setTimeElapsed(unsigned int ms);
	unsigned int getTimeElapsed();
	static FrameRate* getInstance();
};
#endif
