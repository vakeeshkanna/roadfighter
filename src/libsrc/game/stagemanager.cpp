#include "stagemanager.h"

StageManager::StageManager()
{
	loaded = no;
	currentID = 0;
}

StageManager::~StageManager()
{

}

void StageManager::setLoaded(Logical yesno)
{
	loaded = yesno;
}

Logical StageManager::isLoaded()
{
	return loaded;
}

void StageManager::setCurrentID(int id)
{
	currentID = id;
}

int StageManager::getCurrentID()
{
	return currentID;
}
