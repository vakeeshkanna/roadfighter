#include "yellowcar.h"

YellowCar::YellowCar()
{

}

YellowCar::YellowCar(char *name)
:NonRivalCar(name)
{
	addImage(ImageInfo("yellowcar", ROADFIGHTER_IMAGES_DIR, "yellowcar.bmp", 4, 32));
	myType = YELLOW_CAR;
	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(9);
	setStretchedHeight(16);
	initMe();
	setSpeed(200);
	initImages();
}

YellowCar::~YellowCar()
{

}

void YellowCar::initMe()
{
	active = yes;
	state = ALIVE;
	currentFrame = frames[CAR_ALIVE_FRAME];
}

void YellowCar::bumpAction()
{
	slide();
}

