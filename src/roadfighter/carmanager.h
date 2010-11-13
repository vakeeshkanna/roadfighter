#ifndef CAR_MANAGER_H
#define CAR_MANAGER_H

#include "interactive-object-manager.h"
#include "non-rivalcar.h"
#include "rivalcar.h"

#define POOL_COUNT 14

class CarManager : InteractiveObjectManager
{
public:
	CarManager();
	~CarManager();
	virtual void init();
	virtual void reinit();
	virtual void findPositionToSpawn(Car *car);
	virtual void spawn();
	virtual void spawnBonusCar();
	virtual void spawnTruckCar();
	virtual void display();
	virtual void update();
	virtual void cleanup();
	void setLastCarX(double lastX);
	double getLastCarX();
	void setLastCarY(double lastY);
	double getLastCarY();
	void checkCollision(vector<InteractiveObject*> &ObjectsOnScreen);
	void setCanSpawnNewCars(Logical spawn);
	Logical isCanSpawnNewCars();
	void destroyAllCars();
	Logical allCarsDestroyed();
	void setCurrentStage(int current);
	int getCurrentStage();

	vector < Car *> cars;
	Logical canSpawnNewCars;
	int currentStage;
};

#endif
