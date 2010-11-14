#include "oggsound-manager.h"
#include "log-class.h"
OggSoundManager* OggSoundManager::osManager = NULL;

OggSoundManager::OggSoundManager()
{
	oggSounds.clear();
}

OggSoundManager::~OggSoundManager()
{

}

OggSoundManager* OggSoundManager::getInstance()
{
	if(osManager == NULL)
		osManager = new OggSoundManager();

	return osManager;
}

void OggSoundManager::addSound(int id, string fpath, string fname)
{
	fpath += fname;
	//add file extension
	fpath += ".OGG";

	LPDIRECTSOUND8  ds = SE->getDirectSound();
	CDirectSoundOgg *sound = new CDirectSoundOgg(fpath, ds);
	oggSounds.push_back(sound);
}

void OggSoundManager::play(int id, Logical async, Logical looping)
{
	CDirectSoundOgg *sound  = oggSounds.at(id);
	sound->Play(async, looping);
}

void OggSoundManager::stop(int id)
{
	CDirectSoundOgg *sound  = oggSounds.at(id);
	sound->Stop();
}

Logical OggSoundManager::isPlaying(int id)
{
	CDirectSoundOgg *sound  = oggSounds.at(id);
	return sound->IsPlaying();
}

void OggSoundManager::stopAllSounds()
{
	int i;

	for(i = 0; i < oggSounds.size(); i++)
	{
		CDirectSoundOgg *sound  = oggSounds.at(i);
		if(sound != NULL)
		{
			sound->Stop();
		}
	}
}

void OggSoundManager::cleanup()
{
}
