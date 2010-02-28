#include "truck.h"

Truck::Truck()
{

}

Truck::Truck(char *name)
:NonRivalCar(name)
{
	addImage(ImageInfo("truck", ROADFIGHTER_IMAGES_DIR, "truck.bmp", 4, 32, 64));
	myType = TRUCK_CAR;
	init();
	setWidth(32);
	setHeight(64);
	setStretchedWidth(20);
	setStretchedHeight(64);
	initMe();
	setSpeed(200);
	initImages();
}

Truck::~Truck()
{

}

void Truck::initMe()
{
	active = yes;
	state = ALIVE;
	currentFrame = frames[CAR_ALIVE_FRAME];
}

void Truck::bumpAction()
{

}

