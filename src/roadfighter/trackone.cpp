#include "trackone.h"

TrackInfo TrackOne::trackInfo[TRACK_ONE_TURNS] =
{
	{540,NONE,11},
	{25,NORMAL_LEFT,11},
	{540,NONE,11},
	{25,NORMAL_RIGHT,11},
	{540,NONE,11},
	{25,NORMAL_LEFT,11},
	{540,NONE,11},
	{25,NORMAL_RIGHT,11},
	{540,NONE,11},
};

TrackOne::TrackOne()
{

}

TrackOne::~TrackOne()
{

}

void TrackOne::init()
{
}

TrackOne::TrackOne(string trackName, int cellSize):Track(trackName, cellSize)
{
	addImage(ImageInfo("Flying Route", ROADFIGHTER_IMAGES_DIR, "flying_route_track.bmp"));
}

void TrackOne::generateTable()
{
	int i,j;
	int count;

	height = STAGE_ONE_HEIGHT;

	//Make sure to reflect the numCellsX and numCellsY to correspond to current stage
	numCellsX = 11;
	numCellsY = (getHeight() / BOUNDARY_CELL_SIZE);
	numTurns = TRACK_ONE_TURNS - 1;

	Track::generateTable(trackInfo, 71, 8);

	int roadWidth = trackInfo[0].roadWidth;
	char bigRow[20];

	count = 0;

	for(i = numCellsY - 1; i >= 0 ; i--)
	{
		for(j = 0; j < roadWidth; j++)
		{
			if(j == 0)
			{
				bigRow[j] = TRACKONE_BOUNL;
			}
			else if(j == roadWidth - 1)
			{
				bigRow[j] = TRACKONE_BOUNR;
			}
			else if(j == roadWidth / 2 && (i < numCellsY - VP->getNumCellsY() - 1))
			{
				if(count < 3)
				{
					bigRow[j] = TRACKONE_STRIP;
					count++;
				}
				else if(count > 2 && count < 5)
				{
					bigRow[j] = TRACKONE_ROAD;
					count++;
				}
				else
					count = 0;
			}
			else
				bigRow[j] = TRACKONE_ROAD;
		}
		memcpy(cellArray[i], bigRow, roadWidth);
	}
	//paste finish line
	cellArray[VP->getNumCellsY() - 6][2] = TRACKONE_FINISH;
	cellArray[VP->getNumCellsY() - 6][3] = TRACKONE_FINISH;
	cellArray[VP->getNumCellsY() - 6][4] = TRACKONE_FINISH;
	cellArray[VP->getNumCellsY() - 6][5] = TRACKONE_FINISH;
	cellArray[VP->getNumCellsY() - 6][6] = TRACKONE_FINISH;
	cellArray[VP->getNumCellsY() - 6][7] = TRACKONE_FINISH;
	cellArray[VP->getNumCellsY() - 6][8] = TRACKONE_FINISH;
//
//	//paste start line
	cellArray[numCellsY - VP->getNumCellsY() + 3][2] = TRACKONE_START1;
	cellArray[numCellsY - VP->getNumCellsY() + 3][3] = TRACKONE_START2;
	cellArray[numCellsY - VP->getNumCellsY() + 3][4] = TRACKONE_START3;
	cellArray[numCellsY - VP->getNumCellsY() + 3][5] = TRACKONE_START4;
	cellArray[numCellsY - VP->getNumCellsY() + 3][6] = TRACKONE_START5;
	cellArray[numCellsY - VP->getNumCellsY() + 3][7] = TRACKONE_START6;

	cellArray[numCellsY - VP->getNumCellsY() + 4][2] = TRACKONE_START7;
	cellArray[numCellsY - VP->getNumCellsY() + 4][3] = TRACKONE_START8;
	cellArray[numCellsY - VP->getNumCellsY() + 4][4] = TRACKONE_START9;
	cellArray[numCellsY - VP->getNumCellsY() + 4][5] = TRACKONE_START10;
	cellArray[numCellsY - VP->getNumCellsY() + 4][6] = TRACKONE_START11;
	cellArray[numCellsY - VP->getNumCellsY() + 4][7] = TRACKONE_START12;

}

void TrackOne::load()
{
	generateTable();
	lprintf("Track object created with following parameters: %d %d\n",BOUNDARY_CELL_SIZE * 3,numCellsY);
	lprintf("Track numCellsX numCellsY = %d %d\n",numCellsX,numCellsY);

	loadImage(1);
}

void TrackOne::display()
{
	Track::display();
}
