#include "obstacles.h"
#include <math.h>
#include <time.h>

Obstacles::Obstacles()
{

}

Obstacles::Obstacles(char *n,obstacleType obs):InteractiveObject(n)
{
	obsType = obs;
	objectType = "Obstacle";
	switch(obsType)
	{
	case WATER_SPOTS:
		addImage(ImageInfo("waterspot","waterspot.bmp",1));
		break;

	case OIL_SPOTS:
		addImage(ImageInfo("oilspot","oilspot.bmp",1));
		break;

	case MAIN_HOLE:
		addImage(ImageInfo("mainhole","mainhole.bmp",1));
		break;

	}
	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(15);
	setStretchedHeight(16);
	initMe();
}

Obstacles::~Obstacles()
{

}

void Obstacles::initMe()
{
	double x,y;
	int row, col;

	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );

	RoadFighterViewport *rfvp = (RoadFighterViewport*)VP;
	//row = rand() % rfvp->getNumCellsY() + 1;
	//col = rand() % rfvp->track->guardRail[950].roadWidth + 1;]
	x = rfvp->track->guardRail[950].left.getX() + BOUNDARY_CELL_SIZE * 2;
	y = rfvp->track->guardRail[950].left.getY();
	setXPosWC(x);
	setYPosWC(y);
	state = ALIVE;
}

void Obstacles::setObstacleType(obstacleType o)
{
	obsType = o;
}

obstacleType Obstacles::getObstacleType()
{
	return obsType;
}
