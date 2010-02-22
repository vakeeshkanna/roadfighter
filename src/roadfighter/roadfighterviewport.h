#ifndef ROADFIGHTERVIEWPORT_H
#define ROADFIGHTERVIEWPORT_H
#include "viewport.h"
#include "background.h"
#include "track.h"

class RoadFighterViewport : public Viewport
{
	friend class Object;
	
public:
	Track *track;
	
	RoadFighterViewport();
	RoadFighterViewport(int width, int height, int cellsize);
	~RoadFighterViewport();
//	void init(BackGround *background);
//	void update(double moverate);
	void setTrack(Track *track);
	Track* getTrack();
};
#endif


