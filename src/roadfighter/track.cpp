#include "track.h"
#include "background.h"

Track::Track()
{
	guardRail = NULL;
	leftSideBarrier = NULL;
	rightSideBarrier = NULL;
	numTurns = 0;
}

Track::~Track()
{
	if(guardRail != NULL)
	{
		delete []guardRail;
		delete guardRail;
	}

	if(leftSideBarrier != NULL)
	{
		delete []leftSideBarrier;
		delete leftSideBarrier;
	}

	if(rightSideBarrier != NULL)
	{
		delete []rightSideBarrier;
		delete rightSideBarrier;
	}
}

void Track::init()
{
}

Track::Track(string trackName, int cellSize):Stage(cellSize)
{
	name = trackName;
	guardRail = NULL;
	leftSideBarrier = NULL;
	rightSideBarrier = NULL;
	numTurns = 0;
}

void Track::unload()
{
	if(cellArray != NULL)
	{
		for(int i = 0; i < getNumCellsY(); i++)
			delete cellArray[i];
		delete cellArray;
		cellArray = NULL;
	}
//	if(guardRail != NULL)
//	{
//		delete []guardRail;
//		delete guardRail;
//	}
//	if(tiledImage != NULL)
//	{
//		delete tiledImage;
//		tiledImage = NULL;
//	}
}

void Track::setName(string trackName)
{
	name = trackName;
}

string Track::getName()
{
	return name;
}

void Track::generateTable(TrackInfo *trackInfo, int trackStartX, int trackStartY)
{
	int i,j;
	int x,y;
	TrackInfo *currentTrackInfo;

	currentTrackInfo = trackInfo;

	//Forcing contigous memory allocation
	cellArray = new char*[numCellsY];
	//cellArray[0] = new int[numCellsX * numCellsY];
	for (i = 0; i < numCellsY; i++)
	{
		cellArray[i] = new char[numCellsX];//cellArray[0] + i * numCellsY;
	}
	guardRail = new GuardRailCell[numCellsY];
	leftSideBarrier = new LineClass[numTurns + 1];
	rightSideBarrier = new LineClass[numTurns + 1];
	//Prepare the x,y coordinates for each cell
	//First read trackOneInfo for this track
	int turnAngle,numBlocks,roadWidth;
	int count = numCellsY - 1;
	x = trackStartX;
	y = trackStartY;
	for (i = numTurns; i >= 0; i--)
	{
		currentTrackInfo[i].startTurnY = y;
		numBlocks = currentTrackInfo[i].numCells;
		turnAngle = currentTrackInfo[i].curve;
		roadWidth = currentTrackInfo[i].roadWidth;
		lprintf("Index = %d, TurnAngle = %d, NumBlocks = %d\n",i,turnAngle,numBlocks);

		leftSideBarrier[i].setP1(x + 1 ,y);
		rightSideBarrier[i].setP1(BOUNDARY_CELL_SIZE * roadWidth + x - 1 ,y);
		for(j = 0;j < numBlocks;j++)
		{
			switch(turnAngle)
			{
				case NONE:
				break;

				case NORMAL_LEFT:
					x -= 1;
				break;

				case NORMAL_RIGHT:
					x += 1;
				break;

				case STEEP_LEFT:
					x -= 2;
				break;

				case STEEP_RIGHT:
					x += 2;
				break;

				default:
					assert(0);
				break;
			}

			guardRail[count].left.setX(x);
			guardRail[count].left.setY(y);

			guardRail[count].right.setX(x + roadWidth * BOUNDARY_CELL_SIZE);
			guardRail[count].right.setY(y);

			guardRail[count].roadWidth = roadWidth;
			y += BOUNDARY_CELL_SIZE;
			count--;
		}
		currentTrackInfo[i].endTurnY = y;
		leftSideBarrier[i].setP2(x + 1, y);
		rightSideBarrier[i].setP2(BOUNDARY_CELL_SIZE * roadWidth + x - 1, y);
	}
}

void Track::setNumTurns(int nTurns)
{
	numTurns = nTurns;
}

int Track::getNumTurns()
{
	return numTurns;
}

void Track::display()
{
	int i,j,x,y;
	int curRow;
	int limitY;
	int partialTopY = 0,partialBottomY = 0;
	int curPixelLine =  (int)VP->getCurPixLine();
	Logical usingMoreCells = no;

	RECT srcRct,dstRct;
	limitY = VP->getNumCellsY();
	if(curPixelLine % BOUNDARY_CELL_SIZE != 0)
	{
		//Need to handle the case when cells are partially displayed
		limitY++;
		partialBottomY = curPixelLine % BOUNDARY_CELL_SIZE;
		partialTopY = BOUNDARY_CELL_SIZE - partialBottomY;
		usingMoreCells = yes;
	}
	curRow = curPixelLine / BOUNDARY_CELL_SIZE;
	for (i = 0; i < limitY; i++)
	{
		for (j = 0; j < MAX_CELL_TRACK; j++)
		{
			x = cellArray[curRow + i][j] % getNumCellsImageX();
			y = cellArray[curRow + i][j] / getNumCellsImageX();
			srcRct = Stage::getRect(x,y,BOUNDARY_CELL_SIZE);

			dstRct.top = i * BOUNDARY_CELL_SIZE ;
			dstRct.left =  (int)guardRail[curRow + i].left.getX() + j * BOUNDARY_CELL_SIZE;
			dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE;
			dstRct.right = dstRct.left + BOUNDARY_CELL_SIZE;

			if(usingMoreCells && i == 0)
			{
				srcRct.top += partialTopY;
				dstRct.top = i * BOUNDARY_CELL_SIZE;
				dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE - partialBottomY;
			}

			if(usingMoreCells && (i != 0) && (i != limitY - 1) )
			{
				dstRct.top = i * BOUNDARY_CELL_SIZE - partialBottomY;
				dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE;
			}

			if(usingMoreCells && (i == limitY - 1))
			{
				srcRct.bottom -= partialBottomY;
				dstRct.top = i * BOUNDARY_CELL_SIZE - partialBottomY;
				dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE - partialTopY;
			}
			//Copy the contents to the temp buffer
			VP->buffer->Blt(&dstRct,tiledImage,&srcRct,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY,&RE->ddbltfx);
		}
	}
}
