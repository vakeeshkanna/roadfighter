#include "roadfighter-non-interactive-object-manager.h"

RoadFighterNonInteractiveObjectManager::RoadFighterNonInteractiveObjectManager()
{
	objects.clear();
	progressCar = NULL;
	superman = NULL;
	checkpoint = NULL;
	goal = NULL;
}

RoadFighterNonInteractiveObjectManager::~RoadFighterNonInteractiveObjectManager()
{

}

void RoadFighterNonInteractiveObjectManager::init()
{
	NonInteractiveObject *finishFlag = new NonInteractiveObject("finish flag");
	finishFlag->addImage(ImageInfo("finishflag","finishflag.bmp",1));
	finishFlag->init();
	finishFlag->setX(START_FLAG_X);
	finishFlag->setY(START_FLAG_Y);
	objects.push_back(finishFlag);

	NonInteractiveObject *startFlag = new NonInteractiveObject("start flag");
	startFlag->addImage(ImageInfo("startflag","startflag.bmp",1));
	startFlag->init();
	startFlag->setX(FINISH_FLAG_X);
	startFlag->setY(FINISH_FLAG_Y);
	objects.push_back(startFlag);

	progressCar = new NonInteractiveObject("progress car");
	progressCar->addImage(ImageInfo("progresscar","progresscar.bmp",1));
	progressCar->init();
	progressCar->setX(PROGRESS_CAR_X);
	progressCar->setY(PROGRESS_CAR_Y);
	objects.push_back(progressCar);

	superman = new NonInteractiveObject("super man");
	superman->addImage(ImageInfo("superman","superman.bmp",1));
	superman->init();
	superman->setX(40);
	superman->setY(224);
	objects.push_back(superman);

	checkpoint = new NonInteractiveObject("checkpoint");
	checkpoint->addImage(ImageInfo("checkpoint","checkpoint.bmp",1));
	checkpoint->init();
	checkpoint->setX(90);
	checkpoint->setY(75);

	goal = new NonInteractiveObject("goal");
	goal->addImage(ImageInfo("goal","goal.bmp",1));
	goal->init();
	goal->setX(109);
	goal->setY(75);
}

void RoadFighterNonInteractiveObjectManager::reinit()
{
	progressCar->setX(FINISH_FLAG_X);
	progressCar->setY(FINISH_FLAG_Y);

	superman->setX(40);
	superman->setY(224);
}

void RoadFighterNonInteractiveObjectManager::display()
{
	unsigned int i;
	for(i = 0; i < objects.size(); i++)
	{
		NonInteractiveObject *obj = objects.at(i);
		obj->display();
	}
}

void RoadFighterNonInteractiveObjectManager::manage()
{

}

void RoadFighterNonInteractiveObjectManager::cleanup()
{

}

NonInteractiveObject* RoadFighterNonInteractiveObjectManager::getProgressCar()
{
	return progressCar;
}

NonInteractiveObject* RoadFighterNonInteractiveObjectManager::getSuperman()
{
	return superman;
}

void RoadFighterNonInteractiveObjectManager::displayCheckpoint()
{
	checkpoint->display();
}

void RoadFighterNonInteractiveObjectManager::displayGoal()
{
	goal->display();
}
