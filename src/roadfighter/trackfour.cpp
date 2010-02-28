#include "trackfour.h"

TrackInfo TrackFour::trackInfoLeft[TRACK_FOUR_TURNS] =
{
//straight
	{382, NONE, 9},

//turn9
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//turn8
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn7
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//turn6
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//turn5
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn4
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//turn3
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn2
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn1
	{28, NORMAL_RIGHT, 9},
	{1, NORMAL_LEFT, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9}
};

TrackInfo TrackFour::trackInfoRight[TRACK_FOUR_TURNS] =
{
//straight
	{382, NONE, 9},

//turn9
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//turn8
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn7
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//turn6
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//turn5
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn4
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//turn3
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn2
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9},

//turn1
	{27, NORMAL_RIGHT, 9},
	{2, NONE, 9},
	{28, NORMAL_LEFT, 9},

//straight
	{381, NONE, 9}
};

TrackFour::TrackFour()
{
}

TrackFour::~TrackFour()
{

}

void TrackFour::init()
{
}

TrackFour::TrackFour(string trackName, int cellSize):Track(trackName, cellSize)
{
	addImage(ImageInfo("Flying Route 4", ROADFIGHTER_IMAGES_DIR, "flying_route_track_4.bmp"));
}

void TrackFour::generateTable()
{
	int i,j;
	int leftStripCount, rightStripCount;

	height = STAGE_FOUR_HEIGHT;

	//Make sure to reflect the numCellsX and numCellsY to correspond to current stage
	numCellsX = 25;
	numCellsY = (getHeight() / BOUNDARY_CELL_SIZE);
	numTurns = TRACK_FOUR_TURNS - 1;

	generateTable(trackInfoLeft, trackInfoRight, 106, 8);

	int roadWidth = trackInfoLeft[0].roadWidth;
	char bigRow[25];

	leftStripCount = 0;
	rightStripCount = 0;

	for(i = numCellsY - 1; i >= 0 ; i--)
	{
		for(j = 0; j < roadWidth; j++)
		{
			if(j == 0)
			{
				bigRow[j] = TRACKFOUR_BOUNL;
			}
			else if(j == roadWidth - 1)
			{
				bigRow[j] = TRACKFOUR_BOUNR;
			}
			else if(j == 2)
			{
				if(leftStripCount < 3)
				{
					bigRow[j] = TRACKFOUR_STRIP;
					leftStripCount++;
				}
				else if(leftStripCount > 2 && leftStripCount < 5)
				{
					bigRow[j] = TRACKFOUR_ROAD;
					leftStripCount++;
				}
				else
					leftStripCount = 0;
			}
			else if(j == 5)
			{
				if(rightStripCount < 3)
				{
					bigRow[j] = TRACKFOUR_STRIP;
					rightStripCount++;
				}
				else if(rightStripCount > 2 && rightStripCount < 5)
				{
					bigRow[j] = TRACKFOUR_ROAD;
					rightStripCount++;
				}
				else
					rightStripCount = 0;
			}
			else
				bigRow[j] = TRACKFOUR_ROAD;
		}
		memcpy(cellArray[i], bigRow, roadWidth);
	}

	//paste finish line
	cellArray[VP->getNumCellsY() - 6][2] = TRACKFOUR_FINISH;
	cellArray[VP->getNumCellsY() - 6][3] = TRACKFOUR_FINISH;
	cellArray[VP->getNumCellsY() - 6][4] = TRACKFOUR_FINISH;
	cellArray[VP->getNumCellsY() - 6][5] = TRACKFOUR_FINISH;
	cellArray[VP->getNumCellsY() - 6][6] = TRACKFOUR_FINISH;
}

void TrackFour::generateTable(TrackInfo *trackInfoLeft, TrackInfo *trackInfoRight, int trackStartX, int trackStartY)
{
	int i,j;
	int x,y;
	TrackInfo *currentTrackInfo;
	int turnAngle,numBlocks,roadWidth;
	int count;

	//Forcing contigous memory allocation
	cellArray = new char*[numCellsY];
	//cellArray[0] = new int[numCellsX * numCellsY];
	for (i = 0; i < numCellsY; i++)
	{
		cellArray[i] = new char[numCellsX];//cellArray[0] + i * numCellsY;
	}
	guardRail = new GuardRailCell[numCellsY];

	//left side
	currentTrackInfo = trackInfoLeft;
	leftSideBarrier = new LineClass[numTurns + 1];
	count = numCellsY - 1;
	x = trackStartX;
	y = trackStartY;
	for (i = numTurns; i >= 0; i--)
	{
		numBlocks = currentTrackInfo[i].numCells;
		turnAngle = currentTrackInfo[i].curve;
		roadWidth = currentTrackInfo[i].roadWidth;
		lprintf("Index = %d, TurnAngle = %d, NumBlocks = %d\n",i,turnAngle,numBlocks);

		leftSideBarrier[i].setP1(x + 1 ,y);
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

			y += BOUNDARY_CELL_SIZE;
			count--;
		}
		leftSideBarrier[i].setP2(x + 1, y);
	}


	//right side
	currentTrackInfo = trackInfoRight;
	rightSideBarrier = new LineClass[numTurns + 1];
	count = numCellsY - 1;
	x = trackStartX + roadWidth * BOUNDARY_CELL_SIZE;
	y = trackStartY;
	for (i = numTurns; i >= 0; i--)
	{
		numBlocks = currentTrackInfo[i].numCells;
		turnAngle = currentTrackInfo[i].curve;
		roadWidth = currentTrackInfo[i].roadWidth;
		lprintf("Index = %d, TurnAngle = %d, NumBlocks = %d\n",i,turnAngle,numBlocks);

		rightSideBarrier[i].setP1(x - 1 ,y);
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

			guardRail[count].right.setX(x);
			guardRail[count].right.setY(y);

			guardRail[count].roadWidth = (guardRail[count].right.getX() - guardRail[count].left.getX()) / BOUNDARY_CELL_SIZE;
			y += BOUNDARY_CELL_SIZE;
			count--;
		}
		rightSideBarrier[i].setP2(x - 1, y);
	}
}

void TrackFour::load()
{
	generateTable();
	lprintf("Track object created with following parameters: %d %d\n",BOUNDARY_CELL_SIZE * 3,numCellsY);
	lprintf("Track numCellsX numCellsY = %d %d\n",numCellsX,numCellsY);

	loadImage(1);
}

void TrackFour::display()
{
	Track::display();
}
