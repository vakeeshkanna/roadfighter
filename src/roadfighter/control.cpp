#include "control.h"
#include "soundmanager.h"

Control::Control()
{

}

Control::Control(PlayerCar *p)
{
	player = p;
}

void Control::moveLeft()
{
	if(player->getCarState() == CAR_READY || player->getCarState() == CAR_RUNNING)
		player->moveLeft();
}

void Control::moveRight()
{
	if(player->getCarState() == CAR_READY || player->getCarState() == CAR_RUNNING)
		player->moveRight();
}

void Control::accelerateA()
{
	if(player->getCarState() == CAR_READY || player->getCarState() == CAR_RUNNING)
	{
		player->setSpeedMode(SPEED_MODE_A);
		player->incSpeed();
	}
	else
		brake();
}

void Control::accelerateB()
{
	if(player->getCarState() == CAR_READY || player->getCarState() == CAR_RUNNING)
	{
		player->setSpeedMode(SPEED_MODE_B);
		player->incSpeed();
	}
	else
		brake();
	//player->setSpeed(player->getSpeed() + 100);
}

void Control::brake()
{
	if(!player->isCompletingStage())
	{
		if(player->getCarState() == CAR_READY || player->getCarState() == CAR_RUNNING)
		{
			player->decSpeed(1);
		}
		if(player->isCarSlowingDown())
		{
			SM->play(ROADFIGHER_BRAKES, yes, yes);
		}
		else
		{
			if(SM->isPlaying(ROADFIGHER_BRAKES))
			{
				SM->stop(ROADFIGHER_BRAKES);
			}
		}
	}
}


