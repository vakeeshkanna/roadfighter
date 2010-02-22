
RoadFighterViewport::RoadFighterViewport()
{
	
}

RoadFighterViewport::RoadFighterViewport(int width, int height, int cellsize):
Viewport(width, height, cellsize)
{

}

RoadFighterViewport::~RoadFighterViewport()
{
}

//void RoadFighterViewport::init(BackGround * background)
//{
//	totalPixLine = background->getNumCellsY() * CELL_SIZE;
//	//curPixLine = getCurRow() * CELL_SIZE;
//	curPixLine = totalPixLine - getHeight();// actually how many pixel line remaining
//	lprintf("totalPixelLine = %d\n",totalPixLine);
//}

void RoadFighterViewport::setTrack(Track *trk)
{
	track = trk;
}

Track* RoadFighterViewport::getTrack()
{
	return track;
}

//void RoadFighterViewport::update(double moverate)
//{
//	if(curPixLine > 0)
//		curPixLine -= moverate;
//}

