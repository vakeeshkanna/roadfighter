#include "redcar.h"

RedCar::RedCar()
{

}

RedCar::RedCar(char *name)
:RivalCar(name)
{
	addImage(ImageInfo("redcar","redcar.bmp", 4, 32));
	myType = RED_CAR;
	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(9);
	setStretchedHeight(16);
	initMe();
	setSpeed(200);
	initImages();
}

RedCar::~RedCar()
{

}

void RedCar::initMe()
{
	active = yes;
	state = ALIVE;
	alreadyDodgedOnce = no;
	currentFrame = frames[CAR_ALIVE_FRAME];
}

void RedCar::bumpAction()
{
	slide();
}

void RedCar::dodge()
{
	if(!alreadyDodgedOnce)
	{
		myState = CAR_DODGING;
		lastX = getXPosWC();
		alreadyDodgedOnce = yes;
	}
}

void RedCar::dodging()
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
				setCarState(CAR_RUNNING);
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
				setCarState(CAR_RUNNING);
			}
		}
	}
}
