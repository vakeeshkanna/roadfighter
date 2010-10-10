#include "bonuscar.h"

BonusCar::BonusCar()
{

}

BonusCar::BonusCar(char *name)
:NonRivalCar(name)
{
	addImage(ImageInfo("bonuscar", ROADFIGHTER_IMAGES_DIR, "bonuscar.bmp", 1, 32));
	myType = BONUS_CAR;
	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(15);
	setStretchedHeight(16);
	initMe();
	setSpeed(200);
	initImages();
}

BonusCar::~BonusCar()
{

}

void BonusCar::initMe()
{
	active = yes;
	state = ALIVE;
	currentFrame = frames[CAR_ALIVE_FRAME];
}

void BonusCar::bumpAction()
{
	active = no;
	myState = CAR_DESTROYED;
}
