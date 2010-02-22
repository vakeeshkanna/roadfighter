#ifndef RED_CAR_H
#define RED_CAR_H

#include "redcar.h"

class RedCar : public RivalCar
{
public:
	RedCar();
	RedCar(char *name);
	~RedCar();
	void initMe();
	virtual void bumpAction();
	virtual void dodge();
	virtual void dodging();

private:
	Logical alreadyDodgedOnce;
};
#endif
