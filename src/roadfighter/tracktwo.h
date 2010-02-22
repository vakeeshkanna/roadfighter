#ifndef TRACKTWO_H
#define TRACKTWO_H
#include "track.h"

#define TRACK_TWO_TURNS 1

enum trackTwoCells
{
	TRACKTWO_BOUNL1 = 0,
	TRACKTWO_BOUNL2 = 1,
	TRACKTWO_BOUNR1 = 2,
	TRACKTWO_BOUNR2 = 3,
	TRACKTWO_ROAD = 4,
	TRACKTWO_STRIP = 5,
	TRACKTWO_FINISH = 6
};

class TrackTwo : public Track
{
public:
	TrackTwo();
	TrackTwo(string name, int cellSize);
	~TrackTwo();
	virtual void init();
	virtual void load();
	virtual void generateTable();
	virtual void display();
	static TrackInfo trackInfo[TRACK_TWO_TURNS];
};
#endif


