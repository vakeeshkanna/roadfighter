#ifndef BLUE_CAR_H
#define BLUE_CAR_H

#include "rivalcar.h"

class BlueCar : public RivalCar
{
public:
	BlueCar();
	BlueCar(char *name);
	~BlueCar();
	void initMe();
	virtual void bumpAction();
	virtual void dodge();
	virtual void dodging();

private:
	Logical alreadyDodgedOnce;
};
#endif
