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
	carManager->setCurrentStage(getCurrentStage());
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

	if(carManager->isCanSpawnNewCars())
	{
		carManager->spawn();
	}

	if(getCurrentStage() == ROADFIGHTER_STAGE_3 && getCurrentStage() == ROADFIGHTER_STAGE_4)
	{
		if(getStageCompletedPercentage() % 25 == 0)
		{
			carManager->spawnBonusCar();
		}
	}
	else
	{
		if(getStageCompletedPercentage() % 33 == 0)
		{
			carManager->spawnBonusCar();
		}
	}

	if(getStageCompletedPercentage() % 25 == 0)
	{
		carManager->spawnTruckCar();
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

