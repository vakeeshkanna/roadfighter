#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "wave-class.h"

//Global Handle to SoundEngine
#define SE SoundEngine::getInstance()

class EXPORT SoundEngine
{
	LPDIRECTSOUND lpDS;

public:
	static SoundEngine* sEngine;

	SoundEngine();
	~SoundEngine();
	static SoundEngine* getInstance();
	Logical init(HWND hwnd);
	LPDIRECTSOUNDBUFFER createSoundBuffer(string fname);
	void cleanUp();
};

#endif
