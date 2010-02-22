#include "roadfighter-interactive-object-manager.h"

RoadFighterInteractiveObjectManager::RoadFighterInteractiveObjectManager()
:InteractiveObjectManager()
{
	carObjects.clear();
	otherObjects.clear();
	carManager = new CarManager();
	obstacleManager = new ObstaclesManager();
	stageCompletedPerc = 0;
	currentStage = 1;
}

RoadFighterInteractiveObjectManager::~RoadFighterInteractiveObjectManager()
{

}

void RoadFighterInteractiveObjectManager::init()
{
	unsigned int i;
	carManager->init();
	for(i = 0; i < carManager->cars.size(); i++)
	{
		carObjects.push_back(carManager->cars.at(i));
	}

	obstacleManager->init();
	for(i = 0; i < obstacleManager->obstacles.size(); i++)
	{
		otherObjects.push_back(obstacleManager->obstacles.at(i));
	}

	//lprintf("num of object inside rf manager are %d\n",objects.size());
}

void RoadFighterInteractiveObjectManager::spawn()
{
	//carManager->spawn();
	//obstacleManager->spawn();
}

void RoadFighterInteractiveObjectManager::reinit()
{
	carObjects.clear();
	otherObjects.clear();

	carManager->setCurrentStage(getCurrentStage());
	carManager->reinit();
	init();

}

void RoadFighterInteractiveObjectManager::display()
{
	carManager->display();
	obstacleManager->display();
}

void RoadFighterInteractiveObjectManager::update()
{
	carManager->update();
	obstacleManager->update();
	carManager->checkCollision(otherObjects);
	carManager->checkCollision(carObjects);

	//after every five perc of stage completed see if there is a new car to be spawned
	//if(getStageCompletedPercentage() % 1 == 0)
	{
		carManager->spawn();
	}

	if(getStageCompletedPercentage() % 20 == 0)
	{
		carManager->spawnBonusCar();
	}
}

void RoadFighterInteractiveObjectManager::cleanup()
{

}

void RoadFighterInteractiveObjectManager::setStageCompletedPercentage(int percent)
{
	stageCompletedPerc = percent;
}

int RoadFighterInteractiveObjectManager::getStageCompletedPercentage()
{
	return stageCompletedPerc;
}

void RoadFighterInteractiveObjectManager::setCurrentStage(int current)
{
	currentStage = current;
}

int RoadFighterInteractiveObjectManager::getCurrentStage()
{
	return currentStage;
}

