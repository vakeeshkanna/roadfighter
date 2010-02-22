#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "interactive-object-manager.h"
#include "playercar.h"

using namespace std;

class PlayerManager : InteractiveObjectManager
{
public:
	PlayerManager();
	PlayerManager(PlayerCar *player);
	~PlayerManager();
	void setPlayer(PlayerCar *player);
	PlayerCar *getPlayer();
	virtual void init();
	virtual void spawn();
	virtual void display();
	virtual void update();
	virtual void cleanup();
	void reinit();
	void checkCollision(vector<InteractiveObject*> &ObjectsOnScreen);

private:
	PlayerCar *player;
};
#endif
