#include "blueaggressivecar.h"

BlueAggressiveCar::BlueAggressiveCar()
{

}

BlueAggressiveCar::BlueAggressiveCar(char *name)
:RivalCar(name)
{
	addImage(ImageInfo("blueaggressivecar", ROADFIGHTER_IMAGES_DIR, "blueaggressivecar.bmp", 4, 32));
	myType = BLUEAGGRESSIVE_CAR;
	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(9);
	setStretchedHeight(16);
	initMe();
	setSpeed(200);
	initImages();
}

BlueAggressiveCar::~BlueAggressiveCar()
{

}

void BlueAggressiveCar::initMe()
{
	active = yes;
	state = ALIVE;
	currentFrame = frames[CAR_ALIVE_FRAME];
}

void BlueAggressiveCar::bumpAction()
{
	slide();
}

void BlueAggressiveCar::dodge()
{
	myState = CAR_DODGING;
	lastX = getXPosWC();
}

void BlueAggressiveCar::dodging()
{
	if(getCarState() == CAR_DODGING)
	{
		PlayerCar * playercar = (PlayerCar*)PC;
		if(getXPosWC() <= PC->getXPosWC())
		{
			setSlideDirection(DIRECTION_RIGHT);
		}
		else
		{
			setSlideDirection(DIRECTION_LEFT);
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
