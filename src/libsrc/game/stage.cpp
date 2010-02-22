#include "stage.h"

Stage::Stage()
{

}
//At the time of calling this constructor do pass in STAGE_WIDTH,MAX_STAGE_HEIGHT as width and height of the buffer
Stage::Stage(int cSize)
{
	cellSize = cSize;
	numImages = 0;
	tiledImage = NULL;
	cellArray = NULL;
}

Stage::~Stage()
{
	for (int i = 0; i < getNumCellsY(); i++) {
		if(cellArray[i] != NULL) {
			delete []cellArray[i];
		}
	}
	if(getNumCellsY() > 0)
		delete [] cellArray;
}

void Stage::loadImage(int imageIndex)
{
	int bmpWidth,bmpHeight;

	//First check if tileImage surface is freed if not free it before creating a new one
	tiledImage = RE->createBitmapSurface(images.at(imageIndex - 1).getPath().c_str(),&bmpWidth,&bmpHeight);

	setNumCellsImageX((bmpWidth - (bmpWidth / cellSize))  / cellSize);
	setNumCellsImageY((bmpHeight - (bmpHeight / cellSize))  / cellSize);
	lprintf("Image Loaded = %s\n",images.at(imageIndex - 1).getName().c_str());
	lprintf("setNumCellsImageX setNumCellsImageY = %d %d\n",getNumCellsImageX(),getNumCellsImageY());
}

void Stage::setNumCellsX(int nx)
{
	numCellsX = nx;
}

int Stage::getNumCellsX()
{
	return numCellsX;
}

void Stage::setNumCellsY(int ny)
{
	numCellsY = ny;
}


int Stage::getNumCellsY()
{
	return numCellsY;
}

RECT Stage::getRect(int x,int y,int cellsize)
{
	RECT retRct;
	retRct.left = x * cellsize + ((x == 0)? 1 : x + 1);
	retRct.top = y * cellsize + ((y == 0)? 1 : y + 1);
	retRct.right = retRct.left + cellsize;
	retRct.bottom = retRct.top + cellsize;
	return retRct;
}

RECT Stage::getRect(int x,int y,int cellsizex, int cellsizey)
{
	RECT retRct;
	retRct.left = x * cellsizex + ((x == 0)? 1 : x + 1);
	retRct.top = y * cellsizey + ((y == 0)? 1 : y + 1);
	retRct.right = retRct.left + cellsizex;
	retRct.bottom = retRct.top + cellsizey;
	return retRct;
}

void Stage::setNumCellsImageX(int nx)
{
	numCellsImageX = nx;
}

int Stage::getNumCellsImageX()
{
	return numCellsImageX;
}

void Stage::setNumCellsImageY(int ny)
{
	numCellsImageY = ny;
}


int Stage::getNumCellsImageY()
{
	return numCellsImageY;
}

void Stage::setCellSize(int newCellSize)
{
	cellSize = newCellSize;
}
int Stage::getCellSize()
{
	return cellSize;
}

void Stage::setWidth(int h)
{
	height = h;
}

int Stage::getWidth()
{
	return height;
}

void Stage::setHeight(int h)
{
	height = h;
}

int Stage::getHeight()
{
	return height;
}

void Stage::addImage(ImageInfo image)
{
	images.push_back(image);
	numImages++;
}
