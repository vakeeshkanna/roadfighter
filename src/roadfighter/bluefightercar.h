#ifndef BLUE_FIGHTER_CAR_H
#define BLUE_FIGHTER_CAR_H

#include "rivalcar.h"

class BlueFighterCar : public RivalCar
{
public:
	BlueFighterCar();
	BlueFighterCar(char *name);
	~BlueFighterCar();
	void initMe();
	virtual void bumpAction();
	virtual void dodge();
	virtual void dodging();
};
#endif
