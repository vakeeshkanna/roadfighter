#include "trackone.h"

TrackInfo TrackTwo::trackInfo[TRACK_TWO_TURNS] =
{
	{1750,NONE,11}
};

TrackTwo::TrackTwo()
{
}

TrackTwo::~TrackTwo()
{

}

void TrackTwo::init()
{
}

TrackTwo::TrackTwo(string trackName, int cellSize):Track(trackName, cellSize)
{
	addImage(ImageInfo("Flying Route 2","flying_route_track_2.bmp"));
}

void TrackTwo::generateTable()
{
	int i,j;
	int count;

	height = STAGE_TWO_HEIGHT;

	//Make sure to reflect the numCellsX and numCellsY to correspond to current stage
	numCellsX = 11;
	numCellsY = (getHeight() / BOUNDARY_CELL_SIZE);
	numTurns = TRACK_TWO_TURNS - 1;

	Track::generateTable(trackInfo, 69, 8);

	int roadWidth = trackInfo[0].roadWidth;
	char bigRow[20];

	count = 0;
	for(i = numCellsY - 1; i >= 0 ; i--)
	{
		for(j = 0; j < roadWidth; j++)
		{
			if(j == 0)
			{
				if(i % 2 == 0)
				{
					bigRow[j] = TRACKTWO_BOUNL1;
				}
				else
				{
					bigRow[j] = TRACKTWO_BOUNL2;
				}
			}
			else if(j == roadWidth - 1)
			{
				if(i % 2 == 0)
				{
					bigRow[j] = TRACKTWO_BOUNR1;
				}
				else
				{
					bigRow[j] = TRACKTWO_BOUNR2;
				}
			}
			else if(j == roadWidth / 2)
			{
				if(count < 3)
				{
					bigRow[j] = TRACKTWO_STRIP;
					count++;
				}
				else if(count > 2 && count < 5)
				{
					bigRow[j] = TRACKTWO_ROAD;
					count++;
				}
				else
					count = 0;
			}
			else
				bigRow[j] = TRACKTWO_ROAD;
		}
		memcpy(cellArray[i], bigRow, roadWidth);
	}

	//paste finish line
	cellArray[VP->getNumCellsY() - 6][2] = TRACKTWO_FINISH;
	cellArray[VP->getNumCellsY() - 6][3] = TRACKTWO_FINISH;
	cellArray[VP->getNumCellsY() - 6][4] = TRACKTWO_FINISH;
	cellArray[VP->getNumCellsY() - 6][5] = TRACKTWO_FINISH;
	cellArray[VP->getNumCellsY() - 6][6] = TRACKTWO_FINISH;
	cellArray[VP->getNumCellsY() - 6][7] = TRACKTWO_FINISH;
	cellArray[VP->getNumCellsY() - 6][8] = TRACKTWO_FINISH;
}

void TrackTwo::load()
{
	generateTable();
	lprintf("Track object created with following parameters: %d %d\n",BOUNDARY_CELL_SIZE * 3,numCellsY);
	lprintf("Track numCellsX numCellsY = %d %d\n",numCellsX,numCellsY);

	loadImage(1);
}

void TrackTwo::display()
{
	Track::display();
}
