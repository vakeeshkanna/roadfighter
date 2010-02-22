#ifndef RIVALCAR_H
#define RIVALCAR_H
#include "car.h"
#include "roadfighter.h"

enum rivalCarType
{
	RIVALCAR_RED,
	RIVALCAR_BLUE,
	RIVALCAR_BLUE_FIGHTER,
	RIVALCAR_BLUE_AGGRESSIVE,
};

class RivalCar : public Car
{
public:
	RivalCar();
	RivalCar(char *);
	virtual ~RivalCar();
	void initMe() = 0;
	virtual void bumpAction() = 0;
	virtual Logical isNearPlayerCar();
	virtual void dodge() = 0;
	virtual void dodging() = 0;
};
#endif

