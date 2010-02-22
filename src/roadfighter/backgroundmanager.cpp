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
	switch(id)
	{
	case 1:
		currentBackGround->load("stage1.fmp");
		break;

	case 2:
		currentBackGround->load("stage2.fmp");
		break;

	case 3:
		currentBackGround->load("stage3.fmp");
		//cell size is 32 for stage 3
		currentBackGround->setNumCellsX(currentBackGround->getNumCellsX() * 2);
		currentBackGround->setNumCellsY(currentBackGround->getNumCellsY() * 2);
		break;

	case 4:
		currentBackGround->load("stage4.fmp");
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
