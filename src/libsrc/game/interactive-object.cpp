#include "interactive-object.h"
#include "helper-class.h"
#include "basedefs.h"
InteractiveObject::InteractiveObject()
{
}

InteractiveObject::InteractiveObject(char *newName)
:GameObject(newName)
{
	name = newName;
	isPinnedToVP = no;
}

InteractiveObject::~InteractiveObject()
{

}

void InteractiveObject::setPinnedToViewport(Logical yesno)
{
	isPinnedToVP = yesno;
}

Logical InteractiveObject::isPinnedToViewport()
{
	return isPinnedToVP;
}

void InteractiveObject::init()
{
	int bmpWidth,bmpHeight;
	int numCellsImageX, numCellsImageY;
	RECT srcRct;
	int i,j;
	int x,y;
	int cellSizex, cellSizey;
	if(images.size() > 0)
	{
		for (i = 0; i < getNumImages(); i++)
		{
			if(images.at(i).getNumFrames() == 1)
			{
				frames[numFrames] = RE->createBitmapSurface(images.at(i).getPath().c_str(),&bmpWidth,&bmpHeight);
				numFrames++;
			}
			else
			{
				image = RE->createBitmapSurface(images.at(i).getPath().c_str(),&bmpWidth,&bmpHeight);
				cellSizex = images.at(i).getCellSizeX();
				cellSizey = images.at(i).getCellSizeY();
				// now its time to create frames
				// calculate numCellsImageX and numCellsImageY
				numCellsImageX = floor((double)bmpWidth / cellSizex);
				numCellsImageY = floor((double)bmpHeight / cellSizey);

				for (j = 0; j < images.at(i).getNumFrames(); j++) {
					x = j % numCellsImageX;
					y = j / numCellsImageX;

					srcRct = Stage::getRect(x,y,cellSizex, cellSizey);
					frames[numFrames] = RE->createEmptySurface(cellSizex, cellSizey);
					frames[numFrames]->Blt(NULL,image,&srcRct,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, &RE->ddbltfx);
					numFrames++;
				}
			}
		}

		//point to first image
		currentFrame = frames[0];
		if(images.at(0).getNumFrames() == 1)
		{
			setNumCellsX((bmpWidth - (bmpWidth % CELL_SIZE))  / CELL_SIZE);
			setNumCellsY((bmpHeight - (bmpHeight % CELL_SIZE))  / CELL_SIZE);
			setWidth(bmpWidth);
			setHeight(bmpHeight);
		}
		else
		{
			setNumCellsX((cellSizex - (cellSizex % CELL_SIZE)) / CELL_SIZE);
			setNumCellsY((cellSizey - (cellSizey % CELL_SIZE)) / CELL_SIZE);
			setWidth(cellSizex);
			setHeight(cellSizey);
		}
	}
}

void InteractiveObject::display()
{
	int x,y;
	y = getYPosSC();
	if(y != NOT_INSIDE_VIEWPORT)
	{
		//RECT srcRect = getDimension();
		RECT dstRect;

		x = getXPosSC();

		dstRect.top = y;
		dstRect.left = x;
		dstRect.bottom = dstRect.top + getHeight();
		dstRect.right = dstRect.left + getWidth();

		VP->buffer->Blt(&dstRect,currentFrame,NULL,DDBLTFAST_WAIT | DDBLT_KEYSRC,&RE->ddbltfx);
		//RE->DDrawBack->BltFast(x,y,currentFrame,&srcRect,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
//	string name = getName();
	//if(name.compare("Oil Spot") == 0)
	//	lprintf("ypossc = %d\n", y);
}

void InteractiveObject::cleanup()
{

}

void InteractiveObject::initMe()
{

}

void InteractiveObject::setObjectType(string oType)
{
	objectType = oType;
}

string InteractiveObject::getObectType()
{
	return objectType;
}

RECT InteractiveObject::getExtents()
{
	RECT rct;
	rct.left = getXPosWC() + (getWidth() - getStretchedWidth()) /2 ;
	rct.top = getYPosWC() + (getHeight() - getStretchedHeight()) /2 ;
	rct.right = rct.left + getStretchedWidth();
	rct.bottom = rct.top - getStretchedHeight();
	return rct;
}

RECT InteractiveObject::getFullExtents()
{
	RECT rct;
	rct.left = getXPosWC();
	rct.top = getYPosWC();
	rct.right = rct.left + getWidth();
	rct.bottom = rct.top - getHeight();
	return rct;
}

void InteractiveObject::setNumCellsX(int nx)
{
	numCellsX = nx;
}

int InteractiveObject::getNumCellsX()
{
	return numCellsX;
}

void InteractiveObject::setNumCellsY(int ny)
{
	numCellsY = ny;
}

int InteractiveObject::getNumCellsY()
{
	return numCellsY;
}

void InteractiveObject::setXPosSC(double xpos)
{
	xPosSC = xpos;
}

double InteractiveObject::getXPosSC()
{
	return (int)getXPosWC() % (VP->getNumCellsX() * BOUNDARY_CELL_SIZE);
}

void InteractiveObject::setYPosSC(double ypos)
{
	yPosSC = ypos;
}

double InteractiveObject::getYPosSC()
{
	double ypossc = NOT_INSIDE_VIEWPORT;
	if(isPinnedToViewport())
		return yPosSC;
	else
	{
		//display depends on the viewport position as we only display InteractiveObjects which are within viewport
		RECT oRect = getExtents();
		RECT vRect = VP->getExtents();
		if(Helper::checkForOverlap(oRect, vRect))
		{
			ypossc = (int)(vRect.top - getYPosWC());
		}
		return (int)ypossc;
	}
}

Logical InteractiveObject::isFullyInside()
{
	Logical fullyInside = no;
	RECT oRect = getExtents();
	RECT vRect = VP->getExtents();

	if(Helper::checkForOverlapFullyInside(oRect, vRect))
	{
		fullyInside = yes;
	}

	return fullyInside;
}

void InteractiveObject::setXPosWC(double xposwc)
{
	xPos = xposwc;
}

double InteractiveObject::getXPosWC()
{
	return xPos;
}

void InteractiveObject::setYPosWC(double yposwc)
{
	yPos = yposwc;
}

double InteractiveObject::getYPosWC()
{
	return yPos;
	//return (vp->getTrackCurRow() - (vp->getNumCellsY() / BOUNDARY_CELL_SIZE) + 1) * BOUNDARY_CELL_SIZE;
}

void InteractiveObject::setCurRow(int cr)
{
	curRow = cr;
}

int InteractiveObject::getCurRow()
{
	return yPos / BOUNDARY_CELL_SIZE;
}

int InteractiveObject::getWCCurRow()
{
	return VP->getCurRow() - (VP->getNumCellsY() / BOUNDARY_CELL_SIZE) + 1;
}

void InteractiveObject::setStretchedWidth(int width)
{
	stretchedWidth = width;
}

int InteractiveObject::getStretchedWidth()
{
	return stretchedWidth;
}

void InteractiveObject::setStretchedHeight(int height)
{
	stretchedHeight = height;
}

int InteractiveObject::getStretchedHeight()
{
	return stretchedHeight;
}

Logical InteractiveObject::isCollidingWith(InteractiveObject *other)
{
	Logical colliding = no;
	RECT selfRect, otherRect;
	selfRect = getExtents();
	otherRect = other->getExtents();
	if(other->getState() == ALIVE)
	{
		if(Helper::checkForOverlap(selfRect, otherRect))
			colliding = yes;
	}
	return colliding;
}

collisionSide InteractiveObject::isCollidingThenFromWhere(InteractiveObject *other)
{
	collisionSide collside = COLLIDING_NONE;
	RECT selfRect, otherRect;
	string selfName = getName();
	selfRect = getExtents();
	otherRect = other->getExtents();
	if(other->getState() == ALIVE)
	{
		//had to know from which side we have the collision
		if(selfRect.right >= otherRect.left && selfRect.right <= otherRect.right && selfRect.bottom <= otherRect.top && selfRect.bottom >= otherRect.bottom)
			collside = COLLIDING_TOP_LEFT;
		else if(selfRect.left >= otherRect.left && selfRect.left <= otherRect.right && selfRect.bottom <= otherRect.top && selfRect.bottom >= otherRect.bottom)
			collside = COLLIDING_TOP_RIGHT;
		else if(selfRect.right >= otherRect.left && selfRect.right <= otherRect.right && selfRect.top <= otherRect.top && selfRect.top >= otherRect.bottom)
			collside = COLLIDING_BOTTOM_LEFT;
		else if(selfRect.left >= otherRect.left && selfRect.left <= otherRect.right && selfRect.top <= otherRect.top && selfRect.top >= otherRect.bottom)
			collside = COLLIDING_BOTTOM_RIGHT;
	}
	return collside;
}
