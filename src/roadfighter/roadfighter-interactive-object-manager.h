#ifndef ROADFIGHTER_INTERACTIVE_OBJECT_MANAGER_H
#define ROADFIGHTER_INTERACTIVE_OBJECT_MANAGER_H

#include "interactive-object-manager.h"
#include "interactive-object.h"
#include "carmanager.h"
#include "obstaclesmanager.h"

class RoadFighterInteractiveObjectManager : InteractiveObjectManager
{
public:
	RoadFighterInteractiveObjectManager();
	~RoadFighterInteractiveObjectManager();
	virtual void init();
	virtual void spawn();
	virtual void reinit();
	virtual void display();
	virtual void update();
	virtual void cleanup();
	void setStageCompletedPercentage(int percent);
	int getStageCompletedPercentage();
	void setCurrentStage(int current);
	int getCurrentStage();

	vector < InteractiveObject *> carObjects;
	vector < InteractiveObject *> otherObjects;
	CarManager *carManager;

private:
	ObstaclesManager *obstacleManager;
	int stageCompletedPerc;
	int currentStage;
};

#endif
