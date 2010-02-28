#include "bluecar.h"

BlueCar::BlueCar()
{

}

BlueCar::BlueCar(char *name)
:RivalCar(name)
{
	addImage(ImageInfo("bluecar", ROADFIGHTER_IMAGES_DIR, "bluecar.bmp", 4, 32));
	myType = BLUE_CAR;
	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(9);
	setStretchedHeight(16);
	initMe();
	setSpeed(200);
	initImages();
}

BlueCar::~BlueCar()
{

}

void BlueCar::initMe()
{
	active = yes;
	state = ALIVE;
	alreadyDodgedOnce = no;
	currentFrame = frames[CAR_ALIVE_FRAME];
}

void BlueCar::bumpAction()
{
	slide();
}

void BlueCar::dodge()
{
	if(!alreadyDodgedOnce)
	{
		myState = CAR_DODGING;
		lastX = getXPosWC();
		alreadyDodgedOnce = yes;
	}
}

void BlueCar::dodging()
{
	if(getCarState() == CAR_DODGING)
	{
//		PlayerCar * playercar = (PlayerCar*)PC;
//		if(getXPosWC() <= PC->getXPosWC())
//		{
//			direction = RIGHT;
//		}
//		else
//		{
//			direction = LEFT;
//		}

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
