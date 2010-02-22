#include "noninteractive-object.h"

NonInteractiveObject::NonInteractiveObject()
{
	
}

NonInteractiveObject::NonInteractiveObject(char *name)
:GameObject(name)
{
	
}

NonInteractiveObject::~NonInteractiveObject()
{
	
}

void NonInteractiveObject::init()
{
	int bmpWidth,bmpHeight;
	int numCellsImageX, numCellsImageY;
	RECT srcRct;
	int i,j;
	int x,y;
	int cellSizeX, cellSizeY;
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
				cellSizeX = images.at(i).getCellSizeX();
				cellSizeY = images.at(i).getCellSizeY();

				// now its time to create frames
				// calculate numCellsImageX and numCellsImageY
				numCellsImageX = floor((double)bmpWidth / cellSizeX);
				numCellsImageY = floor((double)bmpHeight / cellSizeY);
				
				for (j = 0; j < images.at(i).getNumFrames(); j++) {
					x = j % numCellsImageX;
					y = j / numCellsImageX;
	
					srcRct = Stage::getRect(x,y,cellSizeX, cellSizeY);
					frames[numFrames] = RE->createEmptySurface(cellSizeX, cellSizeY);
					frames[numFrames]->Blt(NULL,image,&srcRct,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, &RE->ddbltfx);
					numFrames++;
				}
			}
		}
	
		//point to first image
		currentFrame = frames[0];
		if(images.at(0).getNumFrames() == 1)
		{
			setWidth(bmpWidth);
			setHeight(bmpHeight);
		}
		else
		{
			setWidth(cellSizeX);
			setHeight(cellSizeY);
		}
	}
}

void NonInteractiveObject::display()
{			
	int x,y;
	RECT srcRect = getDimension();
	RECT dstRect;
	x = getX();
	y = getY();
	
	dstRect.top = y;
	dstRect.left = x;
	dstRect.bottom = dstRect.top + getHeight() / 2;
	dstRect.right = dstRect.left + getWidth() / 2;
	
	VP->buffer->Blt(&dstRect,currentFrame,&srcRect,DDBLTFAST_WAIT | DDBLT_KEYSRC,&RE->ddbltfx);
}

void NonInteractiveObject::cleanup()
{
	
}

void NonInteractiveObject::setX(double x)
{
	xPos = x;
}

double NonInteractiveObject::getX()
{
	return xPos;
}

void NonInteractiveObject::setY(double y)
{
	yPos = y;
}

double NonInteractiveObject::getY()
{
	return yPos;
}
