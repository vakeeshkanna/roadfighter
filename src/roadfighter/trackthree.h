#ifndef TRACKTHREE_H
#define TRACKTHREE_H
#include "track.h"

#define TRACK_THREE_TURNS 44

enum trackThreeCells
{
	TRACKTHREE_BOUNL = 0,
	TRACKTHREE_BOUNR = 1,
	TRACKTHREE_ROAD = 2,
	TRACKTHREE_STRIP = 3,
	TRACKTHREE_FINISH = 4
};

class TrackThree : public Track
{
public:
	TrackThree();
	TrackThree(string name, int cellSize);
	~TrackThree();
	virtual void init();
	virtual void load();
	virtual void generateTable();
	void generateTable(TrackInfo *trackLeft, TrackInfo *trackRight, int trackStartX, int trackStartY);
	virtual void display();
	static TrackInfo trackInfoLeft[TRACK_THREE_TURNS];
	static TrackInfo trackInfoRight[TRACK_THREE_TURNS];
};
#endif


