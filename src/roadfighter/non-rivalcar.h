#ifndef NON_RIVALCAR_H
#define NON_RIVALCAR_H
#include "car.h"
#include "roadfighter.h"

enum nonRivalCarType
{
	NON_RIVALCAR_YELLOW,
	NON_RIVALCAR_TRUCK
};

class NonRivalCar : public Car
{
public:
	NonRivalCar();
	NonRivalCar(char *);
	virtual ~NonRivalCar();
	virtual void bumpAction() = 0;
	virtual void initMe() = 0;
};
#endif

