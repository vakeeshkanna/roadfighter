#ifndef ROADFIGHTER_NON_INTERACTIVE_OBJECT_MANAGER_H
#define ROADFIGHTER_NON_INTERACTIVE_OBJECT_MANAGER_H

#include "non-interactive-object-manager.h"
#include "noninteractive-object.h"

class RoadFighterNonInteractiveObjectManager : NonInteractiveObjectManager
{
public:
	RoadFighterNonInteractiveObjectManager();
	~RoadFighterNonInteractiveObjectManager();
	virtual void init();
	void reinit();
	virtual void display();
	virtual void manage();
	virtual void cleanup();
	void displayCheckpoint();
	void displayGoal();

	NonInteractiveObject* getProgressCar();
	NonInteractiveObject* getSuperman();

private:
	vector < NonInteractiveObject *> objects;
	NonInteractiveObject *progressCar;
	NonInteractiveObject *superman;
	NonInteractiveObject *checkpoint, *goal;
};

#endif
