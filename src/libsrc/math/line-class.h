#ifndef LINECLASS_H
#define LINECLASS_H

#include "basedefs.h"
#include "point-class.h"
#include "rectangle-class.h"
#include "log-class.h"
#include <math.h>
#include <windows.h>

class EXPORT LineClass
{
public:
	LineClass();
	LineClass(Point p1, Point p2);
	LineClass(double x1, double y1, double x2, double y2);
	LineClass(const LineClass &other);
	~LineClass();

	void setP1(Point p1);
	void setP1(double x1, double y1);
	Point getP1();
	void setP2(Point p2);
	void setP2(double x2, double y2);
	Point getP2();
	double length();
	Logical intersects(const LineClass &other);
	Logical intersectsRect(const RectangleClass rect);

private:
	Point p1,p2;
};
#endif
