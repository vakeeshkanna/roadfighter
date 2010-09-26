#include "trackthree.h"

TrackInfo TrackThree::trackInfoLeft[TRACK_THREE_TURNS] =
{
//3280
//	{3280, NONE, 6},

//straight
	{160, NONE, 6},

//turn7
	{2, NONE, 6},
	{20, NORMAL_LEFT, 6},
	{16, NONE, 6},
	{20, NORMAL_RIGHT, 6},
	{2, NONE, 6},

//straight
	{360, NONE, 6},

//turn6
	{2, NONE, 6},
	{20, NORMAL_LEFT, 6},
	{16, NONE, 6},
	{20, NORMAL_RIGHT, 6},
	{2, NONE, 6},

//turn5
	{2, NONE, 6},
	{20, NORMAL_LEFT, 6},
	{16, NONE, 6},
	{20, NORMAL_RIGHT, 6},
	{2, NONE, 6},

//straight
	{360, NONE, 6},

//turn4
	{2, NONE, 6},
	{20, NORMAL_LEFT, 6},
	{16, NONE, 6},
	{20, NORMAL_RIGHT, 6},
	{2, NONE, 6},

//straight
	{360, NONE, 6},

//turn3
	{2, NONE, 6},
	{20, NORMAL_LEFT, 6},
	{16, NONE, 6},
	{20, NORMAL_RIGHT, 6},
	{2, NONE, 6},

//straight
	{360, NONE, 6},

//turn2
	{2, NONE, 6},
	{20, NORMAL_LEFT, 6},
	{16, NONE, 6},
	{20, NORMAL_RIGHT, 6},
	{2, NONE, 6},

//straight
	{360, NONE, 6},

//turn1
	{2, NONE, 6},
	{20, NORMAL_LEFT, 6},
	{16, NONE, 6},
	{20, NORMAL_RIGHT, 6},
	{2, NONE, 6},

//straight
	{390, NONE, 6},

//start
	{22, NORMAL_LEFT, 6},
	{8, NONE, 6}
};

TrackInfo TrackThree::trackInfoRight[TRACK_THREE_TURNS] =
{
//3280
//	{3280, NONE, 6},

//straight
	{160, NONE, 6},

//turn7
	{2, NONE, 6},
	{28, NORMAL_LEFT, 6},
	{1, NONE, 6},
	{7, NORMAL_RIGHT, 6},
	{22, NORMAL_RIGHT, 6},

//straight
	{360, NONE, 6},

//turn6
	{2, NONE, 6},
	{28, NORMAL_LEFT, 6},
	{1, NONE, 6},
	{7, NORMAL_RIGHT, 6},
	{22, NORMAL_RIGHT, 6},

//turn5
	{2, NONE, 6},
	{28, NORMAL_LEFT, 6},
	{1, NONE, 6},
	{7, NORMAL_RIGHT, 6},
	{22, NORMAL_RIGHT, 6},

//straight
	{360, NONE, 6},

//turn4
	{2, NONE, 6},
	{28, NORMAL_LEFT, 6},
	{1, NONE, 6},
	{7, NORMAL_RIGHT, 6},
	{22, NORMAL_RIGHT, 6},

//straight
	{360, NONE, 6},

//turn3
	{2, NONE, 6},
	{28, NORMAL_LEFT, 6},
	{1, NONE, 6},
	{7, NORMAL_RIGHT, 6},
	{22, NORMAL_RIGHT, 6},

//straight
	{360, NONE, 6},

//turn2
	{2, NONE, 6},
	{28, NORMAL_LEFT, 6},
	{1, NONE, 6},
	{7, NORMAL_RIGHT, 6},
	{22, NORMAL_RIGHT, 6},

//straight
	{360, NONE, 6},

//turn1
	{2, NONE, 6},
	{28, NORMAL_LEFT, 6},
	{1, NONE, 6},
	{7, NORMAL_RIGHT, 6},
	{22, NORMAL_RIGHT, 6},

//straight
	{390, NONE, 6},

//start
	{22, NORMAL_LEFT, 6},
	{8, NORMAL_LEFT, 6}
};

TrackThree::TrackThree()
{
}

TrackThree::~TrackThree()
{

}

void TrackThree::init()
{
}

TrackThree::TrackThree(string trackName, int cellSize):Track(trackName, cellSize)
{
	addImage(ImageInfo("Flying Route 3", ROADFIGHTER_IMAGES_DIR, "flying_route_track_3.bmp"));
}

void TrackThree::generateTable()
{
	int i,j;
	int count;

	height = STAGE_THREE_HEIGHT;

	//Make sure to reflect the numCellsX and numCellsY to correspond to current stage
	numCellsX = 25;
	numCellsY = (getHeight() / BOUNDARY_CELL_SIZE);
	numTurns = TRACK_THREE_TURNS - 1;

	generateTable(trackInfoLeft, trackInfoRight, 73, 8);

	int roadWidth = trackInfoLeft[0].roadWidth;
	char bigRow[25];

	count = 0;

	for(i = numCellsY - 1; i >= 0 ; i--)
	{
		for(j = 0; j < roadWidth; j++)
		{
			if(j == 0)
			{
				bigRow[j] = TRACKTHREE_BOUNL;
			}
			else if(j == roadWidth - 1)
			{
				bigRow[j] = TRACKTHREE_BOUNR;
			}
			else if(j == roadWidth / 2)
			{
				if(count < 3)
				{
					if(guardRail[i].showRoadStrip)
					{
						bigRow[j] = TRACKTHREE_STRIP;
					}
					else
					{
						bigRow[j] = TRACKTHREE_ROAD;
					}
					count++;
				}
				else if(count > 2 && count < 5)
				{
					bigRow[j] = TRACKTHREE_ROAD;
					count++;
				}
				else
					count = 0;
			}
			else
				bigRow[j] = TRACKTHREE_ROAD;
		}
		memcpy(cellArray[i], bigRow, roadWidth);
	}

	//paste finish line
	cellArray[VP->getNumCellsY() - 6][1] = TRACKTHREE_FINISH;
	cellArray[VP->getNumCellsY() - 6][2] = TRACKTHREE_FINISH;
	cellArray[VP->getNumCellsY() - 6][3] = TRACKTHREE_FINISH;
	cellArray[VP->getNumCellsY() - 6][4] = TRACKTHREE_FINISH;
}

void TrackThree::generateTable(TrackInfo *trackInfoLeft, TrackInfo *trackInfoRight, int trackStartX, int trackStartY)
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
		currentTrackInfo[i].startTurnY = y;
		numBlocks = currentTrackInfo[i].numCells;
		turnAngle = currentTrackInfo[i].curve;
		roadWidth = currentTrackInfo[i].roadWidth;
		lprintf("Index = %d, TurnAngle = %d, NumBlocks = %d\n",i,turnAngle,numBlocks);

		leftSideBarrier[i].setP1(x + 1 ,y);
		for(j = 0;j < numBlocks;j++)
		{
			if(j % 2 == 0)
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
			}

			if(count < 0)
			{
				lprintf("total count of numcells in trackInfoLeft are inconsistent with count\n");
				assert(0);
			}
			else
			{
				guardRail[count].left.setX(x);
				guardRail[count].left.setY(y);
			}

			y += BOUNDARY_CELL_SIZE;
			count--;
		}
		currentTrackInfo[i].endTurnY = y;
		leftSideBarrier[i].setP2(x + 1, y);
	}

	//right side
	currentTrackInfo = trackInfoRight;
	rightSideBarrier = new LineClass[numTurns + 1];
	count = numCellsY - 1;
	x = trackStartX + 69;
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

			if(count < 0)
			{
				lprintf("total count of numcells in trackInfoRight are inconsistent with count\n");
				assert(0);
			}
			else
			{
				guardRail[count].right.setX(x);
				guardRail[count].right.setY(y);
			}

			guardRail[count].roadWidth = (guardRail[count].right.getX() - guardRail[count].left.getX()) / BOUNDARY_CELL_SIZE;
			//don't show road strip while on turns
			if(numBlocks < 30)
			{
				guardRail[count].showRoadStrip = no;
			}
			y += BOUNDARY_CELL_SIZE;
			count--;
		}
		rightSideBarrier[i].setP2(x - 1, y);
	}
}

void TrackThree::load()
{
	generateTable();
	lprintf("Track object created with following parameters: %d %d\n",BOUNDARY_CELL_SIZE * 3,numCellsY);
	lprintf("Track numCellsX numCellsY = %d %d\n",numCellsX,numCellsY);

	loadImage(1);
}

void TrackThree::display()
{
	int i,j,x,y;
	int curRow;
	int limitY;
	int partialTopY = 0,partialBottomY = 0;
	int curPixelLine =  (int)VP->getCurPixLine();
	Logical usingMoreCellsY = no;
	int roadWidthPixels, roadWidthCells;
	int imageCellIndex = 0;

	RECT srcRct,dstRct;
	limitY = VP->getNumCellsY();
	if(curPixelLine % BOUNDARY_CELL_SIZE != 0)
	{
		//Need to handle the case when cells are partially displayed
		limitY++;
		partialBottomY = curPixelLine % BOUNDARY_CELL_SIZE;
		partialTopY = BOUNDARY_CELL_SIZE - partialBottomY;
		usingMoreCellsY = yes;
	}

	curRow = curPixelLine / BOUNDARY_CELL_SIZE;
	roadWidthPixels = guardRail[curRow].right.getX() - guardRail[curRow].left.getX();
	roadWidthCells = roadWidthPixels / BOUNDARY_CELL_SIZE;

	//do we need an extra block
	if(roadWidthPixels % BOUNDARY_CELL_SIZE != 0)
	{
		roadWidthCells++;
	}

	for (i = 0; i < limitY; i++)
	{
		//paste the road first
		imageCellIndex = 2;
		dstRct.left = (int)guardRail[curRow + i].left.getX()+ BOUNDARY_CELL_SIZE;
		dstRct.right = (int)guardRail[curRow + i].right.getX();

		x = imageCellIndex % getNumCellsImageX();
		y = imageCellIndex / getNumCellsImageX();
		srcRct = Stage::getRect(x, y, BOUNDARY_CELL_SIZE);

		dstRct.top = i * BOUNDARY_CELL_SIZE ;
		dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE;

		if(usingMoreCellsY && i == 0)
		{
			srcRct.top += partialTopY;
			dstRct.top = i * BOUNDARY_CELL_SIZE;
			dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE - partialBottomY;
		}

		if(usingMoreCellsY && (i != 0) && (i != limitY - 1) )
		{
			dstRct.top = i * BOUNDARY_CELL_SIZE - partialBottomY;
			dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE;
		}

		if(usingMoreCellsY && (i == limitY - 1))
		{
			srcRct.bottom -= partialBottomY;
			dstRct.top = i * BOUNDARY_CELL_SIZE - partialBottomY;
			dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE - partialTopY;
		}
		//Copy the contents to the temp buffer
		VP->buffer->Blt(&dstRct,tiledImage,&srcRct,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY,&RE->ddbltfx);

		for (j = 0; j < roadWidthCells; j++)
		{
			if(j == 0)
			{
				imageCellIndex = cellArray[curRow + i][j];
				dstRct.left = (int)guardRail[curRow + i].left.getX();
				dstRct.right = dstRct.left + BOUNDARY_CELL_SIZE;
			}
			else if(j == roadWidthCells - 1)
			{
				imageCellIndex = 1;
				dstRct.left = (int)guardRail[curRow + i].right.getX() - BOUNDARY_CELL_SIZE;
				dstRct.right = (int)guardRail[curRow + i].right.getX();
			}
			else
			{
				if(j == 3)
				{
					imageCellIndex = cellArray[curRow + i][j];
					dstRct.left = (int)guardRail[curRow + i].left.getX()+ j * BOUNDARY_CELL_SIZE;
					dstRct.right = dstRct.left + BOUNDARY_CELL_SIZE;
				}
				if(cellArray[curRow + i][j] == TRACKTHREE_FINISH)
				{
					imageCellIndex = cellArray[curRow + i][j];
					dstRct.left = (int)guardRail[curRow + i].left.getX()+ j * BOUNDARY_CELL_SIZE;
					dstRct.right = dstRct.left + BOUNDARY_CELL_SIZE;
				}
			}

			x = imageCellIndex % getNumCellsImageX();
			y = imageCellIndex / getNumCellsImageX();
			srcRct = Stage::getRect(x, y, BOUNDARY_CELL_SIZE);

			dstRct.top = i * BOUNDARY_CELL_SIZE ;
			dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE;

			if(usingMoreCellsY && i == 0)
			{
				srcRct.top += partialTopY;
				dstRct.top = i * BOUNDARY_CELL_SIZE;
				dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE - partialBottomY;
			}

			if(usingMoreCellsY && (i != 0) && (i != limitY - 1) )
			{
				dstRct.top = i * BOUNDARY_CELL_SIZE - partialBottomY;
				dstRct.bottom = dstRct.top + BOUNDARY_CELL_SIZE;
			}

			if(usingMoreCellsY && (i == limitY - 1))
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
