#include "backgroundmanager.h"

BackGroundManager::BackGroundManager()
{
	currentBackGround = new BackGround();
	currentBackGround->init(RE->DDraw, VP->buffer);
}

BackGroundManager::~BackGroundManager()
{
	if(currentBackGround != NULL)
	{
		delete currentBackGround;
		currentBackGround = NULL;
	}
}

void BackGroundManager::init()
{
}

void BackGroundManager::load(int id)
{
	string mapLocation = ROADFIGHTER_MAPS_DIR;
	switch(id)
	{
	case 1:
		mapLocation += "stage1.fmp";
		currentBackGround->load(mapLocation);
		break;

	case 2:
		mapLocation += "stage2.fmp";
		currentBackGround->load(mapLocation);
		break;

	case 3:
		mapLocation += "stage3.fmp";
		currentBackGround->load(mapLocation);
		break;

	case 4:
		mapLocation += "stage4.fmp";
		currentBackGround->load(mapLocation);
		break;

	default:
		assert(0);
	}

	if(currentBackGround != NULL)
	{
		setCurrentID(id);
		setLoaded(yes);
	}
}

void BackGroundManager::unload()
{
	if(currentBackGround != NULL && isLoaded())
	{
		currentBackGround->unload();
		setLoaded(no);
	}
}

void BackGroundManager::cleanup()
{

}
