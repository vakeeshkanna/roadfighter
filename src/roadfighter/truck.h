#ifndef TRUCK_H
#define TRUCK_H

class Truck : public NonRivalCar
{
public:
	Truck();
	Truck(char* name);
	~Truck();
	virtual void initMe();
	virtual void bumpAction();
};
#endif
