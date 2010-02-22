#include "playermanager.h"

PlayerManager::PlayerManager()
{
	player = new PlayerCar("player");
	player->setInstance(player);
}

PlayerManager::~PlayerManager()
{
	if(player != NULL)
	{
		delete player;
		player = NULL;
	}
}

PlayerManager::PlayerManager(PlayerCar *p)
{
	player = p;
}

void PlayerManager::setPlayer(PlayerCar *p)
{
	player = p;
}

PlayerCar *PlayerManager::getPlayer()
{
	return player;
}

void PlayerManager::init()
{

}

void PlayerManager::spawn()
{
	player->spawn();
}

void PlayerManager::reinit()
{
	if(player != NULL)
	{
		player->setViewPort((RoadFighterViewport*)VP);
		player->initMe();
	}
}

void PlayerManager::display()
{
	if(player != NULL)
	{
		player->display();
	}
}

void PlayerManager::update()
{
	if(player->getCarState() == CAR_SLIDING)
	{
		player->sliding();
		player->move();
	}
	else if(player->getCarState() == CAR_SPINNING)
	{
		player->spinning();
		player->move();
	}
	else if(player->getCarState() == CAR_DESTROYING)
	{
		player->destroying();
	}
	else if(player->getCarState() == CAR_DESTROYED)
	{
		player->spawn();
	}
	else
	{
		player->move();
	}
}

void PlayerManager::cleanup()
{

}

void PlayerManager::checkCollision(vector< InteractiveObject* > &objectsonScreen)
{
	unsigned int i;
	InteractiveObject *interactiveObj;
	Car* car;
	Obstacles *obstacle;
	for(i = 0 ; i < objectsonScreen.size(); i++)
	{
		interactiveObj = objectsonScreen.at(i);
		collisionSide collSide = player->isCollidingThenFromWhere(interactiveObj);
		if(collSide && interactiveObj->getOnScreen())
		{
			lprintf("collision detected\n");
			//Handle all obstacle cases.
			if(interactiveObj->getObectType().compare("Obstacle") == 0)
			{
				obstacle = (Obstacles*)interactiveObj;
				obstacleType myObsType = obstacle->getObstacleType();
				interactiveObj->setState(READY);
				interactiveObj->setOnScreen(no);
				player->bumpAction(myObsType);
			}
			//Handle all car cases.
			else
			{
				car = (Car*)interactiveObj;

				if(car->getCarType() != BONUS_CAR)
				{
					//In the case of car, we are interested in from where we are having the collision
					switch(collSide)
					{
					case COLLIDING_TOP_LEFT:
						player->setSlideDirection(DIRECTION_LEFT);
						break;

					case COLLIDING_TOP_RIGHT:
						player->setSlideDirection(DIRECTION_RIGHT);
						break;

					case COLLIDING_BOTTOM_LEFT:
						player->setSlideDirection(DIRECTION_LEFT);
						break;

					case COLLIDING_BOTTOM_RIGHT:
						player->setSlideDirection(DIRECTION_RIGHT);
						break;
					}
				}

				carType mycarType = car->getCarType();
				if(car->isActive())
				{
					car->setSlideDirection(player->getSlideDirection() == DIRECTION_LEFT ? DIRECTION_RIGHT : DIRECTION_LEFT);
					car->bumpAction();
					player->bumpAction(mycarType);
				}
			}
		}
	}
}
