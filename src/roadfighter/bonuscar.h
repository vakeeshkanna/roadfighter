#ifndef BONUS_CAR_H
#define BONUS_CAR_H

class BonusCar : public NonRivalCar
{
public:
	BonusCar();
	BonusCar(char* name);
	~BonusCar();
	virtual void bumpAction();
	virtual void initMe();
};
#endif
