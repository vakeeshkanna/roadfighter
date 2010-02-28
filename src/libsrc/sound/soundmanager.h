#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#define MAX_SOUNDS 50
#include "soundengine.h"

//Global Handle to SoundManager
#define SM SoundManager::getInstance()

class EXPORT SoundManager
{
public:
	SoundManager();
	~SoundManager();
	static SoundManager* getInstance();
	void addSound(int id, string fpath, string fname);
	void play(int id, Logical async = yes, Logical looping = no);
	void stop(int id);
	Logical isPlaying(int id);
	void stopAllSounds();
	void cleanup();

	static SoundManager* sManager;
private:
	LPDIRECTSOUNDBUFFER lpDSSOUNDS[MAX_SOUNDS];

};

#endif
