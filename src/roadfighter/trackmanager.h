#ifndef TRACKMANAGER_H
#define TRACKMANAGER_H

#include "stagemanager.h"
#include "track.h"
#include "trackone.h"
#include "tracktwo.h"
#include "trackthree.h"
#include "trackfour.h"



class EXPORT TrackManager : public StageManager
{
public:
	TrackManager();
	~TrackManager();
	void init();
	void load(int id);
	void unload();
	void cleanup();

	Track *currentTrack;
};
#endif
