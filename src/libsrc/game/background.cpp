#include "background.h"

BackGround::BackGround()
{
	cellSize = 0;
	DDraw = NULL;
	BackBuffer = NULL;
	mapFile = "";
	offsetX = 0;
	offsetY = 0;
	loaded = no;
}

BackGround::BackGround(int cSize)
{
	cellSize = cSize;
	DDraw = NULL;
	BackBuffer = NULL;
	mapFile = "";
	offsetX = 0;
	offsetY = 0;
	loaded = no;
}

BackGround::~BackGround()
{
	unload();
}

void BackGround::init(LPDIRECTDRAW7 DD, LPDIRECTDRAWSURFACE7 BB)
{
	MapSetDrawArea (0, 0, 256, 224);
	MapSetVal(MPY_8BITTOPINK, 0);
	DDraw = DD;
	BackBuffer = BB;
}

void BackGround::load(string map)
{
	mapFile = map;
	unload();
	if(DDraw != NULL && BackBuffer != NULL)
	{
		if(!MapLoad((char *)mapFile.c_str(), DDraw))
		{
			loaded = yes;
			numCellsX = MapGetVal(MPY_MAPWIDTH);
			numCellsY = MapGetVal(MPY_MAPHEIGHT);
		}
		else
		{
			loaded = no;
		}
	}
}

void BackGround::unload()
{
	if(isLoaded())
	{
		MapFreeMem();
		loaded = no;
	}
}

void BackGround::display()
{
	if(isLoaded())
	{
		MapDrawBG(BackBuffer, getOffsetX(), getOffsetY());
	}
}

void BackGround::setCellSize(int newCellSize)
{
	cellSize = newCellSize;
}
int BackGround::getCellSize()
{
	return cellSize;
}

void BackGround::setNumCellsX(int nx)
{
	numCellsX = nx;
}

int BackGround::getNumCellsX()
{
	return numCellsX;
}

void BackGround::setNumCellsY(int ny)
{
	numCellsY = ny;
}

int BackGround::getNumCellsY()
{
	return numCellsY;
}

void BackGround::setOffsetX(int offx)
{
	offsetX = offx;
}

int BackGround::getOffsetX()
{
	return offsetX;
}

void BackGround::setOffsetY(int offy)
{
	offsetY = offy;
}

int BackGround::getOffsetY()
{
	return offsetY;
}

void BackGround::setOffsetXY(int offx, int offy)
{
	offsetX = offx;
	offsetY = offy;
}

Logical BackGround::isLoaded()
{
	return loaded;
}
