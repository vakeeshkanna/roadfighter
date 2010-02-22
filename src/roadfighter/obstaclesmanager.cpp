#include "obstaclesmanager.h"

ObstaclesManager::ObstaclesManager()
{
	obstacles.clear();
}

ObstaclesManager::~ObstaclesManager()
{

}

void ObstaclesManager::init()
{

	//create objects in advance on the pool
	Obstacles *obstacle  = NULL;
	obstacleType listType[3] = {
							WATER_SPOTS ,
							OIL_SPOTS,
							MAIN_HOLE
						  };
	int i;

	for(i = 2; i < 3; i++)
	{
		switch(listType[i])
		{
		case WATER_SPOTS:
			obstacle = new Obstacles("Water Spot", WATER_SPOTS);
			break;

		case OIL_SPOTS:
			obstacle = new Obstacles("Oil Spot", OIL_SPOTS);
			break;

		case MAIN_HOLE:
			obstacle = new Obstacles("Main Hole", MAIN_HOLE);
			break;

		default:
			assert(0);
		}

		if(obstacle != NULL)
			obstacles.push_back(obstacle);
	}
}

void ObstaclesManager::reinit()
{

}

void ObstaclesManager::spawn()
{
//	Obstacles *obstacle  = NULL;
//	switch(1)
//	{
//		case 1:
//			obstacle = new Obstacles("Oil Spot",OIL_SPOTS);
//		break;
//	}
//
//	if(obstacle != NULL)
//		obstacles.push_back(obstacle);
}

void ObstaclesManager::display()
{
	unsigned int i;
	Obstacles *obstacle;

	for(i = 0; i < obstacles.size(); i++)
	{
		obstacle = obstacles.at(i);
		if(obstacle->getOnScreen())
			obstacle->display();
	}
}

void ObstaclesManager::update()
{
	unsigned int i;
	Obstacles *obstacle;

	for(i = 0; i < obstacles.size(); i++)
	{
		obstacle = obstacles.at(i);
		if(obstacle->getState() == ALIVE && obstacle->getYPosSC() != NOT_INSIDE_VIEWPORT)
		{
			obstacle->setOnScreen(yes);
		}
	}
}

void ObstaclesManager::cleanup()
{

}
