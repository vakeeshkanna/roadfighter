#ifndef CONTROL_H
#define CONTROL_H
#include "playercar.h"

class Control
{
	PlayerCar *player;

public:
	Control();
	Control(PlayerCar*);
	~Control();
	void moveLeft();
	void moveRight();
	void accelerateA();
	void accelerateB();
	void brake();
};
#endif


