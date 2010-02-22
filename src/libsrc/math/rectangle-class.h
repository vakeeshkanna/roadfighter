#ifndef RECTANGLE_CLASS_H
#define RECTANGLE_CLASS_H

#include "basedefs.h"
#include "log-class.h"
#include <math.h>

class EXPORT RectangleClass
{
public:
	RectangleClass();
	RectangleClass(double left, double top, double right, double bottom);
	~RectangleClass();
	//Logical intersectsRect(const LineClass &other);

	double left, top, right, bottom;
};
#endif
