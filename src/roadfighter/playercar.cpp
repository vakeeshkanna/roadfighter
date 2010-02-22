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
	setViewPort((RoadFighterViewport*)VP);
	addImage(ImageInfo("playercar","playercarimages.bmp",25,32));

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
	fuel = 500;
	VP->setCurPixLine(9000);
	stageCompleted = no;
	completingStage = no;
	atFinishLine = no;
	speed = 0;
	canControl = no;
	canDeductFuel = no;
	currentFrame = frames[PLAYER_CAR_ALIVE_FRAME];
	slideDirection = DIRECTION_NONE;

	setPinnedToViewport(yes);

	double left = vp->track->guardRail[vp->getTotalRows() - getCurRow() - 1].left.getX(), right = vp->track->guardRail[vp->getTotalRows() - getCurRow() - 1].right.getX();
	setYPosWC(48);
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

	RectangleClass rect(ex.left - 0.25, ex.top, ex.right - 0.25, ex.bottom);

	for (i = vp->track->getNumTurns(); i >= 0; i--)
	{
		if(vp->track->leftSideBarrier[i].intersectsRect(rect))
		{
			if(speed > 100.0 || getCarState() == CAR_SLIDING || getCarState() == CAR_SPINNING)
			{
				destroy();
			}
			return;
		}
	}
	setXPosWC(getXPosWC() - 0.25);
}

void PlayerCar::moveRight()
{
	int i;
	RECT ex = getFullExtents();

	RectangleClass rect(ex.left  + 0.25, ex.top, ex.right + 0.25, ex.bottom);

	for (i = vp->track->getNumTurns(); i >= 0; i--)
	{
		if(vp->track->rightSideBarrier[i].intersectsRect(rect))
		{
			if(speed > 100.0 || getCarState() == CAR_SLIDING || getCarState() == CAR_SPINNING)
			{
				destroy();
			}
			return;
		}
	}
	setXPosWC(getXPosWC() + 0.25);
}

void PlayerCar::move()
{
	int i;
	double timeElapsed = 0.0;
	if(!completingStage)
	{
		SM->play(ROADFIGHER_ENGINE_SOUND_LOW, yes, yes);
		if(speed > 0.0)
		{
			//check left side
			RECT ex = getFullExtents();
			RectangleClass leftRect(ex.left  - 0.25, ex.top, ex.right - 0.25, ex.bottom);

			for (i = vp->track->getNumTurns(); i >= 0; i--)
			{
				if(vp->track->leftSideBarrier[i].intersectsRect(leftRect))
				{
					if(speed > 100.0)
					{
						destroy();
						return;
					}
					setXPosWC(getXPosWC() + 1.5);
				}
			}

			//check right side
			RectangleClass rightRect(ex.left  + 0.25, ex.top, ex.right + 0.25, ex.bottom);

			for (i = vp->track->getNumTurns(); i >= 0; i--)
			{
				if(vp->track->rightSideBarrier[i].intersectsRect(rightRect))
				{
					if(speed > 100.0)
					{
						destroy();
						return;
					}
					setXPosWC(getXPosWC() - 1.5);
				}
			}

			timeElapsed = frameTimer.getTimeElapsed();
			if(timeElapsed > 0.0)
			{
				double multiplier = 85*2;
				if(getSpeedMode() != SPEED_MODE_A)
				{
					multiplier = 65;
				}
				double moveRate = multiplier * timeElapsed * (1.25) * speed / 100.0;
				moveRate = FR->getSpeedFactor() * 2 * speed / 100.0;
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
			timeElapsed = frameTimer.getTimeElapsed();
			lastY = getYPosWC();
		}
	}
	else
	{
		static Logical firstTime = yes;
		speed = 150;

		//stop playercar at the finish line, wait and then let the playercar go
		timeElapsed = frameTimer.getTimeElapsed();
		double multiplier = 65;
		double moveRate;// = multiplier * timeElapsed * (1.25) * speed / 100.0;
		moveRate = FR->getSpeedFactor() * 2 * speed / 100.0;

		if(firstTime)
		{
			firstTime = no;
			moveRate = 0;
		}

		if(SM->isPlaying(ROADFIGHER_STAGE_CLEAR) || SM->isPlaying(ROADFIGHER_GAME_CLEAR))
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
	if(SM->isPlaying(ROADFIGHER_BRAKES))
	{
		SM->stop(ROADFIGHER_BRAKES);
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
			SM->play(ROADFIGHER_BONUS_CAR_TAKEN, yes, no);
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
	myState = CAR_SLIDING;
	lastX = getXPosWC();
	lastY = getYPosWC();
	if(SM->isPlaying(ROADFIGHER_ENGINE_SOUND_LOW))
	{
		SM->stop(ROADFIGHER_ENGINE_SOUND_LOW);
	}

	//SM->stop(ROADFIGHER_ENGINE_SOUND_HIGH);
	SM->play(ROADFIGHER_CAR_SLIPPING, yes, yes);
	lastSpeed = speed;
}

void PlayerCar::sliding()
{
	static int count = -1;
	static GameFrame myFrametimer;

	if(count == -1)
	{
		myFrametimer.setFPS(60);
		myFrametimer.init();
		count = 0;
	}

	double timeElapsed = myFrametimer.getTimeElapsed();
	if(timeElapsed > 0.0)
	{
		if(getSlideDirection() == DIRECTION_LEFT)
		{
			moveLeft();
			currentFrame = slideFrames[0];
			if(getXPosWC() == lastX - 15)
				spin();
		}
		else if(getSlideDirection() == DIRECTION_RIGHT)
		{
			moveRight();
			currentFrame = slideFrames[1];
			if(getXPosWC() == lastX + 15)
				spin();
		}
		count++;
		if(count > 5)
			count = -1;
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
	static int count = -1;
	static GameFrame myFrametimer;
	static Logical clockwise = no;

	if(count == -1)
	{
		myFrametimer.setFPS(12);
		myFrametimer.init();

		if(getSlideDirection() == DIRECTION_RIGHT)
		{
			count = 0;
			clockwise = yes;
		}
		else if(getSlideDirection() == DIRECTION_LEFT)
		{
			count = PLAYER_CAR_SPIN_FRAME_END - PLAYER_CAR_SPIN_FRAME_START;
			clockwise = no;
		}
	}

	double timeElapsed = myFrametimer.getTimeElapsed();

	if(timeElapsed > 0.0 && count != -1)
	{
		if(clockwise)
		{
			if(count < PLAYER_CAR_SPIN_FRAME_END - PLAYER_CAR_SPIN_FRAME_START)
			{
				currentFrame = spinFrames[count++];
				Car::moveRight(0.75);
			}
			else
			{
				count = -1;
			}
		}
		else
		{
			if(count > 0)
			{
				currentFrame = spinFrames[count--];
				Car::moveLeft(0.75);
			}
			else
			{
				count = -1;
			}
		}
		decSpeed(5);
		move();
		if(getSpeed() == 0)
		{
			straightenCar();
			count = -1;
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
	if(SM->isPlaying(ROADFIGHER_ENGINE_SOUND_LOW))
	{
		SM->stop(ROADFIGHER_ENGINE_SOUND_LOW);
	}

	if(SM->isPlaying(ROADFIGHER_CAR_SLIPPING))
	{
		SM->stop(ROADFIGHER_CAR_SLIPPING);
	}
	SM->play(ROADFIGHER_CAR_DESTORY, yes, no);
}

void PlayerCar::destroying()
{
	static int count = -1;
	static GameFrame myFrametimer;

	if(count == -1)
	{
		myFrametimer.setFPS(2);
		myFrametimer.init();
		count = 0;
	}

	//Stop the player car
	speed = 0.0;

	double timeElapsed = myFrametimer.getTimeElapsed();
	if(timeElapsed > 0.0)
	{
		lprintf("destroying\n");
		if(count < PLAYER_CAR_DESTROY_FRAME_END - PLAYER_CAR_DESTROY_FRAME_START + 1)
		{
			currentFrame = destroyFrames[count++];
		}
		else
		{
			myState = CAR_DESTROYED;
			count = -1;
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
	myState = CAR_READY;
	slideDirection = DIRECTION_NONE;
	currentFrame = frames[PLAYER_CAR_ALIVE_FRAME];
	speed = 0.0;
	mode = SPEED_MODE_A;

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
	if(SM->isPlaying(ROADFIGHER_CAR_SLIPPING))
	{
		SM->stop(ROADFIGHER_CAR_SLIPPING);
	}
	setCarState(CAR_RUNNING);
	setSlideDirection(DIRECTION_NONE);
	currentFrame = frames[PLAYER_CAR_ALIVE_FRAME];
}
