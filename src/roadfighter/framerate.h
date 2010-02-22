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
	static FrameRate* fRate;

	FrameRate();
	void init(float tfps);
	void setSpeedFactor();
	float getSpeedFactor();
	static FrameRate* getInstance();
};
#endif
