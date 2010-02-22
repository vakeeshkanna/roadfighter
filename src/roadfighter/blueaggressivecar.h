#ifndef BLUE_AGGRESSIVE_CAR_H
#define BLUE_AGGRESSIVE_CAR_H

#include "rivalcar.h"

class BlueAggressiveCar : public RivalCar
{
public:
	BlueAggressiveCar();
	BlueAggressiveCar(char *name);
	~BlueAggressiveCar();
	void initMe();
	virtual void bumpAction();
	virtual void dodge();
	virtual void dodging();
};
#endif
