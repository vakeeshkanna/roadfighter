#ifndef TRACK_H
#define TRACK_H
#include "stage.h"
#include "misc.h"
#include "point-class.h"
#include "guardrailcell.h"
#include "line-class.h"
#include "noninteractive-object.h"

enum trackTurnType
{
	NONE,
	NORMAL_LEFT,
	NORMAL_RIGHT,
	STEEP_LEFT,
	STEEP_RIGHT
};

typedef struct
{
	short int numCells,curve,roadWidth;
	int startTurnY,endTurnY;
}
TrackInfo;

class Track : public Stage
{
public:
	Track();
	Track(string name, int cellSize);
	~Track();
	virtual void init();
	virtual void load() = 0;
	void unload();
	virtual void display();
	void display2();
	virtual void generateTable() = 0;
	void generateTable(TrackInfo *track ,int trackStartX, int trackStartY);
	void setNumTurns(int numTurns);
	int getNumTurns();
	void setName(string name);
	string getName();

	GuardRailCell *guardRail;
	LineClass *leftSideBarrier, *rightSideBarrier;
	int numTurns;
	string name;

	NonInteractiveObject *startLine, *finishLine;

};
#endif


