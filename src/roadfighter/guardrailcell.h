#ifndef GUARDRAILCELL_H_
#define GUARDRAILCELL_H_

#include "point-class.h"

class EXPORT GuardRailCell
{
public:
	GuardRailCell();
	~GuardRailCell();
	int roadWidth;
	Logical showRoadStrip;
	//Point *coord;
	Point left, right;
};
#endif /*GUARDRAILCELL_H_*/
