#include "interactive-object.h"

#ifndef OBSTACLES_H
#define OBSTACLES_H
enum obstacleType
{
	WATER_SPOTS,
	OIL_SPOTS,
	MAIN_HOLE
};

class Obstacles : public InteractiveObject
{
public:
	Obstacles();
	Obstacles(char *,obstacleType obs);
	virtual ~Obstacles();
	void initMe();
	void setObstacleType(obstacleType obs);
	obstacleType getObstacleType();
private:
	obstacleType obsType;
};
#endif
