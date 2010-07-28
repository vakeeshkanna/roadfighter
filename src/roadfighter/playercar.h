#ifndef PLAYER_H
#define PLAYER_H

#define SPEED_LIMIT_A 224.0
#define SPEED_LIMIT_B 400.0

#define PLAYER_CAR_ALIVE_FRAME 0

#define PLAYER_CAR_DESTROY_FRAME_START 20
#define PLAYER_CAR_DESTROY_FRAME_END 23

#define PLAYER_CAR_SPIN_FRAME_START 1
#define PLAYER_CAR_SPIN_FRAME_END 19




#include "car.h"
#include "obstacles.h"
#include "misc.h"
#include "renderingengine.h"
#include "roadfighterviewport.h"
//#include "time-class.h"

enum speedMode
{
	SPEED_MODE_A,
	SPEED_MODE_B
};

enum spinDirectionType
{
	CLOCK_WISE,
	ANTI_CLOCK_WISE
};

//Global Handle to Viewport
#define PC PlayerCar::getInstance()

class PlayerCar : public Car
{
	double speedLimit;
	int fuel, score;
	int rank;
	double lastSpeed;
	int currentStage;
	Logical flawless;
	RoadFighterViewport *vp;
	speedMode mode;
	spinDirectionType spinDirection;
	Logical completingStage, stageCompleted, atFinishLine;
	LPDIRECTDRAWSURFACE7 destroyFrames[PLAYER_CAR_DESTROY_FRAME_END - PLAYER_CAR_DESTROY_FRAME_START + 1];
	LPDIRECTDRAWSURFACE7 spinFrames[PLAYER_CAR_SPIN_FRAME_END - PLAYER_CAR_SPIN_FRAME_START + 1];
	LPDIRECTDRAWSURFACE7 slideFrames[2];
	Logical canControl;
	Logical canDeductFuel;
	int spinFrameIndex;


public:
	PlayerCar();
	PlayerCar(char *name);
	~PlayerCar();
	static void setInstance(PlayerCar *playercar);
	static PlayerCar* getInstance();
	void initMe();
	virtual void initImages();
	void moveLeft();
	void moveRight();
	void setSpeedMode(speedMode mode);
	speedMode getSpeedMode();
	virtual void incSpeed();
	void decSpeed(double byHowMuch = 0.0);
	double getSpeedLimit();
	double getLastSpeed();
	void setFuel(int);
	int getFuel();
	void setScore(int);
	int getScore();
	void move();
	void setRank(int rank);
	int getRank();
	void setFlawless(Logical flawless);
	Logical isFlawless();
	void slowDown();
	void slide();
	void sliding();
	Logical isSliding();
	void setSpinDirection(spinDirectionType spin);
	spinDirectionType getSpinDirection();
	void spin();
	void spinning();
	Logical isSpinning();
	virtual void destroy();
	virtual void destroying();
	void spawn();
	void deductFuel(int howMuch);
	void addFuel();
	Logical isRunningLowOnFuel();
	Logical isCarSlowingDown();
	Logical isTankEmpty();
	void setViewPort(RoadFighterViewport *vp);
	Viewport* getViewPort();
	virtual void bumpAction();
	void bumpAction(obstacleType oType);
	void bumpAction(carType cType);
	void setCompletingStage(Logical completing);
	Logical isCompletingStage();
	void setStageCompleted(Logical completed);
	Logical isStageCompleted();
	void setAtFinishLine(Logical atFinish);
	Logical isAtFinishLine();
	void setCanControl(Logical control);
	Logical isCanControl();
	void setCanDeductFuel(Logical deduct);
	Logical isCanDeductFuel();
	void setCurrentStage(int stage);
	int getCurrentStage();
	void straightenCar();

private:
	static PlayerCar* playercar;
};
#endif

