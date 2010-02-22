#include "trackmanager.h"

TrackManager::TrackManager()
{
	currentTrack = NULL;
}

TrackManager::~TrackManager()
{
	if(currentTrack != NULL)
	{
		delete currentTrack;
		currentTrack = NULL;
	}
}

void TrackManager::init()
{
}

void TrackManager::load(int id)
{
	switch(id)
	{
	case 1:
		currentTrack = new TrackOne("TrackOne", BOUNDARY_CELL_SIZE);
		break;

	case 2:
		currentTrack = new TrackTwo("TrackTwo", BOUNDARY_CELL_SIZE);
		break;

	case 3:
		currentTrack = new TrackThree("TrackThree", BOUNDARY_CELL_SIZE);
		break;

	case 4:
		currentTrack = new TrackFour("TrackFour",BOUNDARY_CELL_SIZE);
		break;

	default:
		assert(0);
	}

	if(currentTrack != NULL)
	{
		unload();
		currentTrack->load();
		setCurrentID(id);
		setLoaded(yes);
	}
}

void TrackManager::unload()
{
	if(currentTrack != NULL && isLoaded())
	{
		currentTrack->unload();
		setLoaded(no);
	}
}

void TrackManager::cleanup()
{

}
