#include "bluefightercar.h"

BlueFighterCar::BlueFighterCar()
{

}

BlueFighterCar::BlueFighterCar(char *name)
:RivalCar(name)
{
	addImage(ImageInfo("bluefightercar","bluefightercar.bmp",4, 32));
	myType = BLUEFIGHTER_CAR;
	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(9);
	setStretchedHeight(16);
	initMe();
	setSpeed(200);
	initImages();
}

BlueFighterCar::~BlueFighterCar()
{

}

void BlueFighterCar::initMe()
{
	active = yes;
	state = ALIVE;
	currentFrame = frames[CAR_ALIVE_FRAME];
}

void BlueFighterCar::bumpAction()
{
	slide();
}

void BlueFighterCar::dodge()
{
	myState = CAR_DODGING;
	lastX = getXPosWC();
}

void BlueFighterCar::dodging()
{
	if(getCarState() == CAR_DODGING)
	{
		if(getSlideDirection() == DIRECTION_NONE)
		{
			srand ( (unsigned int)time(NULL) );
			int leftOrRight = rand() % 2;
			leftOrRight == 0 ? setSlideDirection(DIRECTION_LEFT) : setSlideDirection(DIRECTION_RIGHT);
		}
		if(getSlideDirection() == DIRECTION_LEFT)
		{
			if(getXPosWC() >= getLastX() - 10)
			{
				moveLeft(0.25);
			}
			else
			{
				setSlideDirection(DIRECTION_RIGHT);
			}
		}
		else if(getSlideDirection() == DIRECTION_RIGHT)
		{
			if(getXPosWC() <= getLastX() + 10)
			{
				moveRight(0.25);
			}
			else
			{
				setSlideDirection(DIRECTION_LEFT);
			}
		}
	}
}
