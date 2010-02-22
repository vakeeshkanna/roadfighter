#ifndef OBSTACLES_MANAGER_H
#define OBSTACLES_MANAGER_H

#include "interactive-object-manager.h"
#include "obstacles.h"

class ObstaclesManager : InteractiveObjectManager
{
public:
	ObstaclesManager();
	~ObstaclesManager();
	virtual void init();
	virtual void reinit();
	virtual void spawn();
	virtual void display();
	virtual void update();
	virtual void cleanup();

	vector < Obstacles *> obstacles;
};

#endif
