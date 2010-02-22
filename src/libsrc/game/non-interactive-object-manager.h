#ifndef NONINTERACTIVE_OBJECT_MANAGER_H
#define NONINTERACTIVE_OBJECT_MANAGER_H

#include "noninteractive-object.h"

class NonInteractiveObjectManager
{
public:
	NonInteractiveObjectManager() {};
	~NonInteractiveObjectManager() {};
	virtual void init() = 0;
	virtual void display() = 0;
	virtual void manage() = 0;
	virtual void cleanup() = 0;

private:
	vector < NonInteractiveObject *> objects;
};

#endif
