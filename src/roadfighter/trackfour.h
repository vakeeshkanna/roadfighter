#ifndef TRACKFOUR_H
#define TRACKFOUR_H
#include "track.h"

#define TRACK_FOUR_TURNS 33

enum trackFourCells
{
	TRACKFOUR_BOUNL = 0,
	TRACKFOUR_BOUNR = 1,
	TRACKFOUR_ROAD = 2,
	TRACKFOUR_STRIP = 3,
	TRACKFOUR_FINISH = 4
};

class TrackFour : public Track
{
public:
	TrackFour();
	TrackFour(string name, int cellSize);
	~TrackFour();
	virtual void init();
	virtual void load();
	virtual void generateTable();
	void generateTable(TrackInfo *trackLeft, TrackInfo *trackRight, int trackStartX, int trackStartY);
	virtual void display();
	static TrackInfo trackInfoLeft[TRACK_FOUR_TURNS];
	static TrackInfo trackInfoRight[TRACK_FOUR_TURNS];
};
#endif


