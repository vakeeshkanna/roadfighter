#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

class SoundManager
{
public:
	virtual void addSound(int id, string fpath, string fname) = 0;
	virtual void play(int id, Logical async = yes, Logical looping = no) = 0;
	virtual void stop(int id) = 0;
	virtual Logical isPlaying(int id) = 0;
	virtual void stopAllSounds() = 0;
	virtual void cleanup() = 0;
};

#endif
