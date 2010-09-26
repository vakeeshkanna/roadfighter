#ifndef CAR_H
#define CAR_H
#include "interactive-object.h"
#include "basedefs.h"


#define CAR_ALIVE_FRAME 0

#define CAR_DESTROY_FRAME_START 1
#define CAR_DESTROY_FRAME_END 3

enum carState
{
	CAR_READY,
	CAR_RUNNING,
	CAR_SLIDING,
	CAR_SPINNING,
	CAR_DODGING,
	CAR_DESTROYING,
	CAR_DESTROYED,
};

enum carType
{
	BONUS_CAR,
	YELLOW_CAR,
	RED_CAR,
	BLUE_CAR,
	BLUEFIGHTER_CAR,
	BLUEAGGRESSIVE_CAR,
	TRUCK_CAR
};

enum slideDirectionType
{
	DIRECTION_NONE,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

class Car : public InteractiveObject
{
protected:
	carState myState;
	carType myType;
	double speed;
	Logical active;
	slideDirectionType slideDirection;
	double lastX, lastY;
	int startTurnIndex, endTurnIndex;
	LPDIRECTDRAWSURFACE7 destroyFrames[CAR_DESTROY_FRAME_END - CAR_DESTROY_FRAME_START + 1];


public:
	Car();
	Car::Car(char *,double);
	virtual ~Car();
	virtual void initMe() = 0;
	virtual void initImages();
	void setSpeed(double speed);
	double getSpeed();
	virtual void incSpeed();
	void setCarState(carState state);
	carState getCarState();
	void setCarType(carType myType);
	carType getCarType();
	void setLastX(double lX);
	double getLastX();
	void setLastY(double lY);
	double getLastY();
	void setSlideDirection(slideDirectionType spin);
	slideDirectionType getSlideDirection();
	void setActive(Logical active);
	Logical isActive();
	void slide();
	void sliding();
	Logical isSliding();
	virtual void destroy();
	virtual void destroying();
	virtual void move();
	virtual void moveLeft(double howMuch);
	virtual void moveRight(double howMuch);
	virtual void bumpAction() = 0;
	Logical isRivalCar();
	virtual void spawnAt(double xPos, double yPos);
	void calculateTurnIndex();
	int getStartTurnIndex();
	int getEndTurnIndex();
};
#endif


