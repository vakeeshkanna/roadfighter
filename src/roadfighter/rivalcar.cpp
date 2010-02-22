#include "rivalcar.h"

RivalCar::RivalCar()
{

}

RivalCar::RivalCar(char *n):Car(n,0.0)
{
/*	switch(rivalType)
	{
		case RIVALCAR_BLUE:
			addImage(ImageInfo("bluecar","yellowcar.bmp",1));
			init();
			setWidth(16);
			setHeight(16);
			setStretchedWidth(15);
			setStretchedHeight(16);
			initMe();
			setSpeed(150);
		break;

		case RIVALCAR_BLUE_AGGRESSIVE:
			addImage(ImageInfo("blueaggressivecar","yellowcar.bmp",1));
			init();
			setWidth(16);
			setHeight(16);
			setStretchedWidth(15);
			setStretchedHeight(16);
			initMe();
			setSpeed(150);
		break;

		case RIVALCAR_RED:
			addImage(ImageInfo("redcar","yellowcar.bmp",1));
			init();
			setWidth(16);
			setHeight(16);
			setStretchedWidth(15);
			setStretchedHeight(16);
			initMe();
			setSpeed(150);
		break;

		default:
			assert(0);
	}
*/
}

RivalCar::~RivalCar()
{

}

void RivalCar::initMe()
{

}

Logical RivalCar::isNearPlayerCar()
{
	PlayerCar *playercar = (PlayerCar*)PC;
	double x = getXPosWC(), y = getYPosWC();

	//draw a bounding box around the current car and then if the playercar is inside that bounding box
	RECT rivalRect, playerRect;

	rivalRect.left = x - getWidth();
	rivalRect.top = y + getHeight() * 3;
	rivalRect.right = x + getWidth() + getWidth();
	rivalRect.bottom = y - getHeight() * 3 - getHeight();

	playerRect = playercar->getExtents();

	return	Helper::checkForOverlap(rivalRect, playerRect);
}
