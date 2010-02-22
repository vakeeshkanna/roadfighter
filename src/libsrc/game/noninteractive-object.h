#ifndef NONINTERACTIVEOBJECT_H
#define NONINTERACTIVEOBJECT_H

#include "gameobject.h"

class EXPORT NonInteractiveObject : public GameObject
{

public:
	NonInteractiveObject();
	NonInteractiveObject(char *name);
	~NonInteractiveObject();
	void init();
	void display();
	void cleanup();
	void setX(double x);
	double getX();
	void setY(double y);
	double getY();
	
private:
	double xPos,yPos;
	
};
#endif
