#include "playercar.h"
#include "helper-class.h"
#include "rectangle-class.h"


PlayerCar::PlayerCar()
{

}

PlayerCar::PlayerCar(char *n):Car(n,0.0)
{
	mode = SPEED_MODE_A;
	slideDirection = DIRECTION_NONE;
	fuel = 100;
	score = 0;
	lastSpeed = 401;
	cell = NULL;
	flawless = yes;
	completingStage = yes;
	stageCompleted = no;
	atFinishLine = no;
	canControl = no;
	canDeductFuel = no;
	currentStage = 1;
	spinFrameIndex = -1;
	counterSteeringButtonsWereReleased = no;
	setViewPort((RoadFighterViewport*)VP);
	addImage(ImageInfo("playercar", ROADFIGHTER_IMAGES_DIR, "playercarimages.bmp",25,32));

	init();
	setWidth(16);
	setHeight(16);
	setStretchedWidth(9);
	setStretchedHeight(16);
	initMe();
	initImages();
}


PlayerCar::~PlayerCar()
{
}

PlayerCar* PlayerCar::playercar = NULL;

void PlayerCar::setInstance(PlayerCar* pcar)
{
	playercar = pcar;
}

PlayerCar* PlayerCar::getInstance()
{
	return playercar;
}

void PlayerCar::initMe()
{
	int startX = (vp->getNumCellsX() / 2) - 1;// - getNumCellsX();
	int startY = (vp->getNumCellsY() - getNumCellsY() * 4) + 2;
	flawless = yes;
	fuel = 100;
	score = 0;
	//VP->setCurPixLine(9000);
	stageCompleted = no;
	completingStage = no;
	atFinishLine = no;
	speed = 0;
	canControl = no;
	canDeductFuel = no;
	currentFrame = frames[PLAYER_CAR_ALIVE_FRAME];
	mode = SPEED_MODE_A;
	spinFrameIndex = -1;
	counterSteeringButtonsWereReleased = no;
	straightenCar();
	setPinnedToViewport(yes);
	setYPosWC(48);

	double left = vp->track->guardRail[vp->getTotalRows() - getCurRow() - 1].left.getX(), right = vp->track->guardRail[vp->getTotalRows() - getCurRow() - 1].right.getX();
	setXPosWC(left + (((right - left) / 2) - 2) - 20);
	setYPosSC(startY * BOUNDARY_CELL_SIZE);
	//setXPosWC(230);
	lprintf("playername = %s\n",getName());
	lprintf("xposwc = %lf xpossc = %lf yposwc = %lf ypossc = %lf\n",getXPosWC(), getXPosSC(), getYPosWC(), getYPosSC());
}

void PlayerCar::initImages()
{
	int i, destroyIndex = 0, spinIndex = 0;

	for(i = 0; i < getNumFrames(); i++)
	{
		if(i >= PLAYER_CAR_DESTROY_FRAME_START && i <= PLAYER_CAR_DESTROY_FRAME_END)
		{
			destroyFrames[destroyIndex++] = frames[i];
		}
		if(i >= PLAYER_CAR_SPIN_FRAME_START && i <= PLAYER_CAR_SPIN_FRAME_END)
		{
			spinFrames[spinIndex++] = frames[i];
		}
	}

	//left
	slideFrames[0] = frames[19];
	//right
	slideFrames[1] = frames[1];
}

void PlayerCar::setViewPort(RoadFighterViewport *viewport)
{
	vp = viewport;
}

Viewport* PlayerCar::getViewPort()
{
	return vp;
}

void PlayerCar::moveLeft()
{
	int i;
	RECT ex = getFullExtents();
	double timeElapsed = FR->getTimeElapsed() / 11;

	RectangleClass rect(ex.left - 0.25, ex.top, ex.right - 0.25, ex.bottom);

	for (i = getStartTurnIndex(); i >= getEndTurnIndex(); i--)
	{
		if(vp->track->leftSideBarrier[i].intersectsRect(rect))
		{
			if(speed > 50.0 || getCarState() == CAR_SLIDING || getCarState() == CAR_SPINNING)
			{
				destroy();
			}
			return;
		}
	}
	setXPosWC(getXPosWC() - 0.25 * timeElapsed);
}

void PlayerCar::moveRight()
{
	int i;
	RECT ex = getFullExtents();
	double timeElapsed = FR->getTimeElapsed() / 11;

	RectangleClass rect(ex.left  + 0.25, ex.top, ex.right + 0.25, ex.bottom);

	for (i = getStartTurnIndex(); i >= getEndTurnIndex(); i--)
	{
		if(vp->track->rightSideBarrier[i].intersectsRect(rect))
		{
			if(speed > 50.0 || getCarState() == CAR_SLIDING || getCarState() == CAR_SPINNING)
			{
				destroy();
			}
			return;
		}
	}
	setXPosWC(getXPosWC() + 0.25 * timeElapsed);
}

void PlayerCar::move()
{
	int i;
	unsigned int deltaTime = FR->getTimeElapsed();
	double speedInMilliseconds = (speed / 1.25) / 1000; // pixels per millisecond
	double distanceTraveled = deltaTime * speedInMilliseconds; // distance = speed * time

	if(!completingStage)
	{
		WSM->play(ROADFIGHER_ENGINE_LOW_SOUND, yes, yes);
		if(speed > 0.0)
		{
			//check left side
			RECT ex = getFullExtents();
			RectangleClass leftRect(ex.left  - 0.25, ex.top, ex.right - 0.25, ex.bottom);

			for (i = getStartTurnIndex(); i >= getEndTurnIndex(); i--)
			{
				if(vp->track->leftSideBarrier[i].intersectsRect(leftRect))
				{
					if(speed > 50.0)
					{
						destroy();
						return;
					}
					setXPosWC(getXPosWC() + 1.5 * deltaTime);
				}
			}

			//check right side
			RectangleClass rightRect(ex.left  + 0.25, ex.top, ex.right + 0.25, ex.bottom);

			for (i = getStartTurnIndex(); i >= getEndTurnIndex(); i--)
			{
				if(vp->track->rightSideBarrier[i].intersectsRect(rightRect))
				{
					if(speed > 50.0)
					{
						destroy();
						return;
					}
					setXPosWC(getXPosWC() - 1.5 * deltaTime);
				}
			}

			if(deltaTime > 0.0)
			{
				double moveRate = distanceTraveled;//speed / 100.0;
				if(getSpeedMode() == SPEED_MODE_A)
				{
					moveRate *= 1.5;
				}
				vp->update(moveRate);
				if(yPos < vp->getTotalPixLine() - STAGE_HEIGHT)
				{
					yPos += moveRate;
				}
			}
		}

		if(VP->getCurPixLine() <= 5)
		{
			setYPosWC(10);
			canControl = no;
			canDeductFuel = no;
			setAtFinishLine(yes);
			speed = 0;
			currentFrame = frames[PLAYER_CAR_ALIVE_FRAME];
			setSlideDirection(DIRECTION_NONE);
			setCarState(CAR_RUNNING);
			setCompletingStage(yes);
			lastY = getYPosWC();
		}
	}
	else
	{
		static Logical firstTime = yes;
		speed = 150;

		speedInMilliseconds = (speed / 1.5) / 1000; // pixels per millisecond
		distanceTraveled = deltaTime * speedInMilliseconds; // distance = speed * time

		//stop playercar at the finish line, wait and then let the playercar go
		double moveRate = distanceTraveled;

		if(firstTime)
		{
			firstTime = no;
			moveRate = 0;
		}

		if(WSM->isPlaying(ROADFIGHER_STAGE_CLEAR_SOUND) || WSM->isPlaying(ROADFIGHER_GAME_CLEAR_SOUND))
		{
			yPosSC -= moveRate;
		}
		else
		{
			setStageCompleted(yes);
			setCompletingStage(no);
			firstTime = yes;
		}
	}
}

void PlayerCar::setSpeedMode(speedMode m)
{
	mode = m;
}

speedMode PlayerCar::getSpeedMode()
{
	return mode;
}

double PlayerCar::getLastSpeed()
{
	return lastSpeed;
}

void PlayerCar::incSpeed()
{
	if(WSM->isPlaying(ROADFIGHER_BRAKES_SOUND))
	{
		WSM->stop(ROADFIGHER_BRAKES_SOUND);
	}

	if(myState == CAR_DESTROYED || myState == CAR_SLIDING || myState == CAR_SPINNING)
		return;

	if(speed < getSpeedLimit())
	{
		if(mode == SPEED_MODE_A)
		{
			speed += 2.5;
		}
		else
		{
			speed += 1.5;
		}

		return;
	}

	if(speed > getSpeedLimit())
	{
		if(mode == SPEED_MODE_A)
		{
			//gradually force down the car to the allowed limit within this mode
			decSpeed(5.0);
		}
		else
		{
			speed = SPEED_LIMIT_B;
		}
	}
}

void PlayerCar::decSpeed(double byHowMuch)
{
	lastSpeed = speed;
	if(speed > 0.0)
	{
		if(byHowMuch > 0.0)
			speed -= byHowMuch;
		else
			speed -= 1.0;
	}
	if(speed < 0.0)
	{
		speed = 0.0;
	}
}


double PlayerCar::getSpeedLimit()
{
	if(mode == SPEED_MODE_A)
		speedLimit = SPEED_LIMIT_A;
	else
		speedLimit = SPEED_LIMIT_B;

	return speedLimit;
}

void PlayerCar::setFuel(int f)
{
	fuel = f;

	if(fuel < 0)
		fuel = 0;
}

int PlayerCar::getFuel()
{
	return fuel;
}

void PlayerCar::setScore(int s)
{
	score = s;
}

int PlayerCar::getScore()
{
	return score;
}

void PlayerCar::bumpAction()
{

}

void PlayerCar::bumpAction(obstacleType oType)
{
	switch(oType)
	{
	case WATER_SPOTS:
		slowDown();
		break;

	case OIL_SPOTS:
		slide();
		break;

	case MAIN_HOLE:
		destroy();
		break;
	}
}

void PlayerCar::bumpAction(carType cType)
{
	switch(cType)
	{
		case BONUS_CAR:
			WSM->play(ROADFIGHER_BONUS_CAR_TAKEN_SOUND, yes, no);
			addFuel();
			score += 1000;
		break;

		case YELLOW_CAR:
			slide();
		break;

		case RED_CAR:
			slide();
		break;

		case BLUE_CAR:
			slide();
		break;

		case BLUEFIGHTER_CAR:
			slide();
		break;

		case BLUEAGGRESSIVE_CAR:
			slide();
		break;

		case TRUCK_CAR:
			destroy();
		break;
	}
}

void PlayerCar::setRank(int r)
{
	rank = r;
}

int PlayerCar::getRank()
{
	return rank;
}

void PlayerCar::setFlawless(Logical flaw)
{
	flawless = flaw;
}

Logical PlayerCar::isFlawless()
{
	return flawless;
}

void PlayerCar::slowDown()
{
	speed = speed / 2;
}

void PlayerCar::slide()
{
	//if having a collision while sliding then set the car to be spinning
	if(isSliding())
	{
		spin();
		return;
	}

	myState = CAR_SLIDING;
	lastX = getXPosWC();
	lastY = getYPosWC();
	if(WSM->isPlaying(ROADFIGHER_ENGINE_LOW_SOUND))
	{
		WSM->stop(ROADFIGHER_ENGINE_LOW_SOUND);
	}

	//WSM->stop(ROADFIGHER_ENGINE_SOUND_HIGH);
	WSM->play(ROADFIGHER_CAR_SLIPPING_SOUND, yes, yes);
	lastSpeed = speed;
}

void PlayerCar::sliding()
{
	static Timer frameTimer;
	static Logical timerInitialized = no;
	static Logical counterSteeringStatus = no;

	if(getSlideDirection() == DIRECTION_LEFT)
	{
		if(KEY_UP(VK_RIGHT) && KEY_UP(VK_UP) || KEY_UP(VK_DOWN))
		{
			counterSteeringButtonsWereReleased = yes;
		}
	}
	else if(getSlideDirection() == DIRECTION_RIGHT)
	{
		if(KEY_UP(VK_LEFT) && KEY_UP(VK_UP) || KEY_UP(VK_DOWN))
		{
			counterSteeringButtonsWereReleased = yes;
		}
	}

	frameTimer.forceTickBasedTimer();
	if(!timerInitialized)
	{
		frameTimer.start();
		timerInitialized = yes;
	}

	int timeElapsed = frameTimer.getTicks();

	if(timeElapsed > 30)
	{
		timerInitialized = no;
		if(getSlideDirection() == DIRECTION_LEFT)
		{
			moveLeft();
			currentFrame = slideFrames[0];
			if(getXPosWC() <= lastX - 15)
				spin();
		}
		else if(getSlideDirection() == DIRECTION_RIGHT)
		{
			moveRight();
			currentFrame = slideFrames[1];
			if(getXPosWC() >= lastX + 15)
				spin();
		}
		decSpeed(2);
	}
}

Logical PlayerCar::isSliding()
{
	return myState == CAR_SLIDING;
}

void PlayerCar::spin()
{
	myState = CAR_SPINNING;
	lastY = getYPosWC();
}

void PlayerCar::spinning()
{
	static Timer frameTimer;
	static Logical clockwise = no;
	static Logical timerInitialized = no;

	frameTimer.forceTickBasedTimer();
	if(!timerInitialized)
	{
		frameTimer.start();
		timerInitialized = yes;
	}

	int timeElapsed = frameTimer.getTicks();

	if(spinFrameIndex == -1)
	{
		if(getSlideDirection() == DIRECTION_RIGHT)
		{
			spinFrameIndex = 0;
			clockwise = yes;
		}
		else if(getSlideDirection() == DIRECTION_LEFT)
		{
			spinFrameIndex = PLAYER_CAR_SPIN_FRAME_END - PLAYER_CAR_SPIN_FRAME_START;
			clockwise = no;
		}
	}

	if(timeElapsed > 30)
	{
		timerInitialized = no;
		if(clockwise)
		{
			if(spinFrameIndex < PLAYER_CAR_SPIN_FRAME_END - PLAYER_CAR_SPIN_FRAME_START)
			{
				currentFrame = spinFrames[spinFrameIndex++];
				Car::moveRight(0.75);
			}
			else
			{
				spinFrameIndex = 0;
			}
		}
		else
		{
			if(spinFrameIndex > 0)
			{
				currentFrame = spinFrames[spinFrameIndex--];
				Car::moveLeft(0.75);
			}
			else
			{
				spinFrameIndex = PLAYER_CAR_SPIN_FRAME_END - PLAYER_CAR_SPIN_FRAME_START;
			}
		}
		decSpeed(5);
		move();
		if(getSpeed() == 0)
		{
			straightenCar();
		}
	}
}

Logical PlayerCar::isSpinning()
{
	return myState == CAR_SPINNING;
}

void PlayerCar::destroy()
{
	myState = CAR_DESTROYING;

	flawless = no;

	lastY = getYPosWC();
	if(WSM->isPlaying(ROADFIGHER_ENGINE_LOW_SOUND))
	{
		WSM->stop(ROADFIGHER_ENGINE_LOW_SOUND);
	}

	if(WSM->isPlaying(ROADFIGHER_CAR_SLIPPING_SOUND))
	{
		WSM->stop(ROADFIGHER_CAR_SLIPPING_SOUND);
	}
	WSM->play(ROADFIGHER_CAR_DESTORY_SOUND, yes, no);
}

void PlayerCar::destroying()
{
	if(!destroyTimerInitialized)
	{
		destroyFrameTimer.start();
		destroyTimerInitialized = yes;
	}

	int timeElapsed = destroyFrameTimer.getTicks();

	if(destroyFrameIndex == -1)
	{
		destroyFrameIndex = 0;
	}

	//Stop the player car
	speed = 0.0;

	if(timeElapsed > 300)
	{
		destroyTimerInitialized = no;
		if(destroyFrameIndex < PLAYER_CAR_DESTROY_FRAME_END - PLAYER_CAR_DESTROY_FRAME_START + 1)
		{
			currentFrame = destroyFrames[destroyFrameIndex++];
		}
		else
		{
			myState = CAR_DESTROYED;
			destroyFrameIndex = -1;
			deductFuel(5);
		}
	}
}

void PlayerCar::deductFuel(int howMuch)
{
	fuel -= howMuch;
	if(fuel < 0)
	{
		fuel = 0;
	}
}

void PlayerCar::addFuel()
{
	fuel += 7;
}

Logical PlayerCar::isRunningLowOnFuel()
{
	if(fuel < 10)
		return yes;
	else
		return no;
}

Logical PlayerCar::isCarSlowingDown()
{
	if(lastSpeed > speed)
		return yes;
	else
		return no;
}

Logical PlayerCar::isTankEmpty()
{
	if(fuel <= 0)
		return yes;
	else
		return no;
}

void PlayerCar::setSpinDirection(spinDirectionType sDir)
{
	spinDirection = sDir;
}

spinDirectionType PlayerCar::getSpinDirection()
{
	return spinDirection;
}

void PlayerCar::spawn()
{
	myState = CAR_RUNNING;
	slideDirection = DIRECTION_NONE;
	currentFrame = frames[PLAYER_CAR_ALIVE_FRAME];
	speed = 0.0;
	mode = SPEED_MODE_A;
	spinFrameIndex = -1;

	double left = vp->track->guardRail[vp->getTotalRows() - getCurRow()].left.getX(), right = vp->track->guardRail[vp->getTotalRows() - getCurRow()].right.getX();

	//spawn in the middle of the track
	setXPosWC(left + (((right - left) / 2) - 2));
	setYPosWC(lastY);
}

void PlayerCar::setCompletingStage(Logical completing)
{
	completingStage = completing;
}

Logical PlayerCar::isCompletingStage()
{
	return completingStage;
}

void PlayerCar::setStageCompleted(Logical completed)
{
	stageCompleted = completed;
}

Logical PlayerCar::isStageCompleted()
{
	return stageCompleted;
}

void PlayerCar::setAtFinishLine(Logical atFinish)
{
	atFinishLine = atFinishLine;
}

Logical PlayerCar::isAtFinishLine()
{
	return atFinishLine;
}

void PlayerCar::setCanControl(Logical control)
{
	canControl = control;
}

Logical PlayerCar::isCanControl()
{
	return canControl;
}

void PlayerCar::setCanDeductFuel(Logical deduct)
{
	canDeductFuel = deduct;
}

Logical PlayerCar::isCanDeductFuel()
{
	return canDeductFuel;
}

void PlayerCar::setCurrentStage(int stage)
{
	currentStage = stage;
}

int PlayerCar::getCurrentStage()
{
	return currentStage;
}

void PlayerCar::straightenCar()
{
	if(WSM->isPlaying(ROADFIGHER_CAR_SLIPPING_SOUND))
	{
		WSM->stop(ROADFIGHER_CAR_SLIPPING_SOUND);
	}
	setCarState(CAR_RUNNING);
	setSlideDirection(DIRECTION_NONE);
	currentFrame = frames[PLAYER_CAR_ALIVE_FRAME];
	spinFrameIndex = -1;
}

Logical PlayerCar::AreCounterSteeringButtonsWereReleased()
{
	return counterSteeringButtonsWereReleased;
}

void PlayerCar::setCounterSteeringButtons(Logical released)
{
	counterSteeringButtonsWereReleased = released;
}
