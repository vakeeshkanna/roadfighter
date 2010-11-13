#ifndef OGGSOUND_MANAGER_H
#define OGGSOUND_MANAGER_H

#define MAX_SOUNDS 50

#include "soundmanager.h"
#include "soundengine.h"
#include "ogg-class.h"
#include <vector>

//Global Handle to OggSoundManager
#define OSM OggSoundManager::getInstance()

class EXPORT OggSoundManager : SoundManager
{
public:
	OggSoundManager();
	~OggSoundManager();
	static OggSoundManager* getInstance();
	virtual void addSound(int id, string fpath, string fname);
	virtual void play(int id, Logical async = yes, Logical looping = no);
	virtual void stop(int id);
	virtual Logical isPlaying(int id);
	virtual void stopAllSounds();
	virtual void cleanup();

	static OggSoundManager* osManager;
private:
	vector<CDirectSoundOgg *> oggSounds;
};

#endif
