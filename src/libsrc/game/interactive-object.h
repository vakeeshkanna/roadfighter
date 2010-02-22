#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "renderingengine.h"
#include "cell.h"
#include "string-class.h"
#include "viewport.h"
#include <vector>
#include <math.h>
#include "gameobject.h"
#include "basedefs.h"


enum collisionSide
{
	COLLIDING_NONE,
	COLLIDING_TOP_LEFT,
	COLLIDING_TOP_RIGHT,
	COLLIDING_BOTTOM_LEFT,
	COLLIDING_BOTTOM_RIGHT,

};

#define CELL_SIZE 16
#define BOUNDARY_CELL_SIZE 8
#define NOT_INSIDE_VIEWPORT 555555
class EXPORT InteractiveObject : public GameObject
{
protected:
	Cell **cell;
	int numCellsX,numCellsY;
	double xPos,yPos;
	double xPosSC,yPosSC;
	int curRow;
	Logical isPinnedToVP;
	int stretchedWidth,stretchedHeight;
	string objectType;

public:
	InteractiveObject();
	InteractiveObject(char *name);
	~InteractiveObject();
	void init();
	virtual void initMe();
	void display();
	void cleanup();
	void setObjectType(string objectType);
	string getObectType();
	RECT getExtents();
	RECT getFullExtents();
	void setNumCellsX(int);
	int getNumCellsX();
	void setNumCellsY(int);
	int getNumCellsY();
	void setXPosSC(double);
	double getXPosSC();
	void setYPosSC(double);
	double getYPosSC();
	void setXPosWC(double);
	double getXPosWC();
	void setYPosWC(double);
	double getYPosWC();
	void setCurRow(int);
	int getCurRow();
	int getWCCurRow();
	void setPinnedToViewport(Logical yesno);
	Logical isPinnedToViewport();
	void setStretchedWidth(int width);
	int getStretchedWidth();
	void setStretchedHeight(int height);
	int getStretchedHeight();
	Logical isFullyInside();
	Logical isCollidingWith(InteractiveObject *other);
	collisionSide isCollidingThenFromWhere(InteractiveObject *other);
};
#endif
