#ifndef WAVESOUND_MANAGER_H
#define WAVESOUND_MANAGER_H

#define MAX_SOUNDS 50
#include "soundmanager.h"
#include "soundengine.h"

//Global Handle to WaveSoundManager
#define WSM WaveSoundManager::getInstance()

class EXPORT WaveSoundManager : SoundManager
{
public:
	WaveSoundManager();
	~WaveSoundManager();
	static WaveSoundManager* getInstance();
	virtual void addSound(int id, string fpath, string fname);
	virtual void play(int id, Logical async = yes, Logical looping = no);
	virtual void stop(int id);
	virtual Logical isPlaying(int id);
	virtual void stopAllSounds();
	virtual void cleanup();

	static WaveSoundManager* wsManager;
private:
	LPDIRECTSOUNDBUFFER lpDSSOUNDS[MAX_SOUNDS];

};

#endif
