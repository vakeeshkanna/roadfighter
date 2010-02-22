#ifndef TRACKONE_H
#define TRACKONE_H
#include "track.h"

#define TRACK_ONE_TURNS 9

enum trackOneCells
{
	TRACKONE_BOUNL = 0,
	TRACKONE_BOUNR = 1,
	TRACKONE_ROAD = 2,
	TRACKONE_STRIP = 3,
	TRACKONE_START1 = 4,
	TRACKONE_START2 = 5,
	TRACKONE_START3 = 6,
	TRACKONE_START4 = 7,
	TRACKONE_START5 = 8,
	TRACKONE_START6 = 9,
	TRACKONE_START7 = 14,
	TRACKONE_START8 = 15,
	TRACKONE_START9 = 16,
	TRACKONE_START10 = 17,
	TRACKONE_START11 = 18,
	TRACKONE_START12 = 19,
	TRACKONE_FINISH = 10
};

class TrackOne : public Track
{
public:
	TrackOne();
	TrackOne(string name, int cellSize);
	~TrackOne();
	virtual void init();
	virtual void load();
	virtual void generateTable();
	virtual void display();
	static TrackInfo trackInfo[TRACK_ONE_TURNS];
};
#endif


