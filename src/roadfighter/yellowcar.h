#ifndef YELLOW_CAR_H
#define YELLOW_CAR_H

class YellowCar : public NonRivalCar
{
public:
	YellowCar();
	YellowCar(char* name);
	~YellowCar();
	virtual void initMe();
	virtual void bumpAction();
};
#endif
