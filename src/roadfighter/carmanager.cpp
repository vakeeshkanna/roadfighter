#include "carmanager.h"

CarManager::CarManager()
{
	cars.clear();
	canSpawnNewCars = yes;
	currentStage = 1;
}

CarManager::~CarManager()
{

}

void CarManager::init()
{
	//create objects in advance on the pool
	Car *car  = NULL;
	canSpawnNewCars = yes;

	int stageCarCount[NUM_STAGES] = {9, 8, 8, 8};

	carType listType[NUM_STAGES][POOL_COUNT] = {
			//Stage 1
			{BONUS_CAR, YELLOW_CAR, YELLOW_CAR, YELLOW_CAR, YELLOW_CAR, RED_CAR, BLUE_CAR, BLUEFIGHTER_CAR},
			//Stage 2
			{BONUS_CAR, YELLOW_CAR, YELLOW_CAR, RED_CAR, BLUE_CAR, BLUEFIGHTER_CAR, BLUEAGGRESSIVE_CAR, TRUCK_CAR},
			//Stage 3
			{BONUS_CAR, YELLOW_CAR, YELLOW_CAR, RED_CAR, BLUE_CAR, BLUEFIGHTER_CAR, BLUEAGGRESSIVE_CAR, TRUCK_CAR},
			//Stage 4
			{BONUS_CAR, YELLOW_CAR, YELLOW_CAR, RED_CAR, BLUE_CAR, BLUEFIGHTER_CAR, BLUEAGGRESSIVE_CAR, TRUCK_CAR},
	};
	int i;

	int current = getCurrentStage() - 1;
	for(i = 0; i < stageCarCount[current]; i++)
	{
		switch(listType[current][i])
		{
			case BONUS_CAR:
				car = new BonusCar("Bonus Car");
			break;

			case YELLOW_CAR:
				car = new YellowCar("Yellow Car");
			break;

			case RED_CAR:
				car = new RedCar("Red Car");
			break;

			case BLUE_CAR:
				car = new BlueCar("Blue Car");
			break;

			case BLUEFIGHTER_CAR:
				car = new BlueFighterCar("Blue Fighter Car");
			break;

			case BLUEAGGRESSIVE_CAR:
				car = new BlueAggressiveCar("Blue Aggressive Car");
			break;

			case TRUCK_CAR:
				car = new Truck("Truck");
			break;

			default:
				assert(0);
		}

		if(car != NULL)
			cars.push_back(car);
	}
}

void CarManager::reinit()
{
	int i;
	Car *car;

	for(i = 0; i < cars.size(); i++)
	{
		car = cars.at(i);
		delete car;
	}

	cars.clear();
}

void CarManager::findPositionToSpawn(Car *newCar)
{
	int i;
	RoadFighterViewport *rfvp = (RoadFighterViewport*)VP;
	double x,y;
	int row;
	int startWidth, endWidth;
	static Logical firstTime = yes;
	int rangeX;
	static int currentIndex = 0;
	#define  lastCarCount  20
	static Car *lastCars[lastCarCount] = {NULL, NULL, NULL, NULL, NULL};

	if(firstTime)
	{
		for(i = 0; i < lastCarCount; i++)
		{
			lastCars[i] = new YellowCar();
			lastCars[i]->setXPosWC(0.0);
			lastCars[i]->setYPosWC(0.0);
			lastCars[i]->setState(ALIVE);
		}
	}

	srand ( (unsigned int)time(NULL) );

	newCar->setXPosWC(0.0);
	newCar->setYPosWC(0.0);

	Logical done = no;

	while(!done)
	{
		row = rand() % 500;
		row = rfvp->getCurRow() - row;

		if(row > 0)
		{
			startWidth = rfvp->track->guardRail[row].left.getX();
			endWidth = startWidth + rfvp->track->guardRail[row].roadWidth * BOUNDARY_CELL_SIZE - newCar->getStretchedWidth() * 2;

			rangeX = (endWidth - startWidth) + 1;
			x = rand() % rangeX + startWidth;

			y = rfvp->track->guardRail[row].left.getY();

//			lprintf("currentIndex = %d row = %d  x = %f y = %f\n", currentIndex, row, x, y);

			newCar->setXPosWC(x);
			newCar->setYPosWC(y);

			if(firstTime)
			{
				firstTime = no;
				done = yes;
				lastCars[currentIndex]->setXPosWC(x);
				lastCars[currentIndex]->setYPosWC(y);
				lastCars[currentIndex]->setWidth(newCar->getWidth());
				lastCars[currentIndex]->setHeight(newCar->getHeight());
				lastCars[currentIndex]->setStretchedWidth(newCar->getStretchedWidth());
				lastCars[currentIndex]->setStretchedHeight(newCar->getStretchedHeight());
			}
			else
			{
				//lprintf("newcar left = %f, top = %f, right = %f, bottom = %f\n",newCar->getXPosWC(), newCar->getYPosWC(),newCar->getXPosWC() + newCar->getStretchedWidth(), newCar->getYPosWC() - newCar->getStretchedHeight());
				//lprintf("lastcar left = %f, top = %f, right = %f, bottom = %f\n",lastCar->getXPosWC(), lastCar->getYPosWC(),lastCar->getXPosWC() + lastCar->getStretchedWidth(), lastCar->getYPosWC() - lastCar->getStretchedHeight());
				for(i = 0; i < lastCarCount; i++)
				{
					if(newCar->isCollidingWith(lastCars[i]))
					{
						break;
					}
				}
				if(i == lastCarCount)
				{
					done = yes;
					currentIndex++;
					if(currentIndex > lastCarCount - 1)
						currentIndex = 0;
					lastCars[currentIndex]->setXPosWC(x);
					lastCars[currentIndex]->setYPosWC(y);
					lastCars[currentIndex]->setWidth(newCar->getWidth());
					lastCars[currentIndex]->setHeight(newCar->getHeight());
					lastCars[currentIndex]->setStretchedWidth(newCar->getStretchedWidth());
					lastCars[currentIndex]->setStretchedHeight(newCar->getStretchedHeight());
				}
			}
		}
		else
		{
			done = yes;
		}
	}
}

void CarManager::spawn()
{
	unsigned int i;
	Car *car;

	for(i = 0; i < cars.size(); i++)
	{
		car = cars.at(i);
		if(car->getCarState() == CAR_READY && car->getCarType() != BONUS_CAR)
		{
			car->setSlideDirection(DIRECTION_NONE);
			car->initMe();
			//manage the positions of all the cars
			findPositionToSpawn(car);
			car->setCarState(CAR_RUNNING);
			car->setSpeed(200);
			break;
		}
	}
}

void CarManager::spawnBonusCar()
{
	unsigned int i;
	Car *car;

	car = cars.at(0);
	if(car->getCarState() == CAR_READY)
	{
		car->setSlideDirection(DIRECTION_NONE);
		car->initMe();
		//manage the positions of all the cars
		findPositionToSpawn(car);
		car->setCarState(CAR_RUNNING);
		car->setSpeed(200);
	}
}

void CarManager::display()
{
	unsigned int i;

	for(i = 0; i < cars.size(); i++)
	{
		Car *car = cars.at(i);
		if(car->getOnScreen())
		{
			car->display();
		}
	}
}

void CarManager::update()
{
	unsigned int i;
	Car *car;
	for(i = 0; i < cars.size(); i++)
	{
		car = cars.at(i);
		car->calculateTurnIndex();
//		lprintf("car %d x = %f y = %f\n", i, car->getXPosWC(), car->getYPosWC());
		if(car->getCarState() == CAR_RUNNING && car->getYPosSC() != NOT_INSIDE_VIEWPORT && !car->getOnScreen())
		{
			car->setOnScreen(yes);
			car->reInitFrameTimer();
		}

		if(car->getOnScreen() && car->getYPosSC() == NOT_INSIDE_VIEWPORT)
		{
			car->setCarState(CAR_DESTROYED);
		}

		if(!car->getOnScreen() && !isCanSpawnNewCars() && car->getCarState() == CAR_DESTROYING)
		{
			car->setCarState(CAR_READY);
		}

		if(car->getOnScreen())
		{
			if(car->getCarState() == CAR_DESTROYING)
			{
				car->destroying();
			}
			else if(car->getCarState() == CAR_DESTROYED)
			{
				car->setOnScreen(no);
				if(isCanSpawnNewCars())
				{
					car->setCarState(CAR_READY);
				}
			}
			else if(car->getCarState() == CAR_SLIDING)
			{
				car->sliding();
			}
			else if(car->isRivalCar())
			{
				RivalCar *rivalCar = (RivalCar*)car;
				if(rivalCar->getCarState() == CAR_RUNNING && (rivalCar->isNearPlayerCar() || rivalCar->getCarType() == BLUEFIGHTER_CAR))
				{
					if(rivalCar->getCarState() != CAR_DODGING)
					{
						rivalCar->dodge();
					}
				}

				if(rivalCar->getCarState() == CAR_DODGING)
				{
					rivalCar->dodging();
				}
			}
		}
		car->move();
	}
}

void CarManager::cleanup()
{

}

void CarManager::checkCollision(vector< InteractiveObject* > &objectsonScreen)
{
	unsigned int i, j;;
	InteractiveObject *interactiveObj;
	Car *car;
	Car* otherCar;
	Obstacles *obstacle;
	for(i = 0 ; i < cars.size(); i++)
	{
		car = cars.at(i);
		for(j = 0 ; j < objectsonScreen.size(); j++)
		{
			interactiveObj = objectsonScreen.at(j);

			//preventing collision from itself
			if(interactiveObj->getObectType().compare("Car") == 0 && i == j)
			{
				break;
			}

			Logical colliding = car->isCollidingWith(interactiveObj);
			if(colliding && interactiveObj->getOnScreen())
			{
				//Handle all obstacle cases.
				if(interactiveObj->getObectType().compare("Obstacle") == 0)
				{
					obstacle = (Obstacles*)interactiveObj;
					obstacleType myObsType = obstacle->getObstacleType();
					interactiveObj->setState(READY);
					interactiveObj->setOnScreen(no);
					car->bumpAction();
				}
				//Handle all car cases.
				else
				{
					otherCar = (Car*)interactiveObj;

					if(otherCar->isActive())
					{
						if(car->getCarType() == BONUS_CAR && otherCar->getCarType() != TRUCK_CAR)
						{
							otherCar->destroy();
						}
						else if(otherCar->getCarType() == BONUS_CAR && car->getCarType() != TRUCK_CAR)
						{
							car->destroy();
						}
						else
							if(car->getCarType() != TRUCK_CAR && otherCar->getCarType() != TRUCK_CAR)
						{
							otherCar->destroy();
						}
						else if(car->getCarType() == TRUCK_CAR && otherCar->getCarType() != TRUCK_CAR)
						{
							otherCar->destroy();
						}
						else if(otherCar->getCarType() == TRUCK_CAR && car->getCarType() != TRUCK_CAR)
						{
							car->destroy();
						}
					}
				}
			}
		}
	}
}

void CarManager::setCanSpawnNewCars(Logical spawn)
{
	canSpawnNewCars = spawn;
}

Logical CarManager::isCanSpawnNewCars()
{
	return canSpawnNewCars;
}

void CarManager::destroyAllCars()
{
	int i = 0;
	int totalCars = cars.size();
	Car *car;
	for(i = 0; i < totalCars; i++)
	{
		car = cars.at(i);
		car->destroy();
	}
	setCanSpawnNewCars(no);
}

Logical CarManager::allCarsDestroyed()
{
	int i = 0;
	int totalCars = cars.size();
	Car *car;

	for(i = 0; i < totalCars; i++)
	{
		car = cars.at(i);
		if(car->getCarState() != CAR_DESTROYED && car->getOnScreen() && car->getCarType() != BONUS_CAR)
		{
			return no;
		}
	}

	return yes;
}


void CarManager::setCurrentStage(int current)
{
	currentStage = current;
}

int CarManager::getCurrentStage()
{
	return currentStage;
}
