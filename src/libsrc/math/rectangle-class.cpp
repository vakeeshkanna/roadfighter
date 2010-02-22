#include "rectangle-class.h"

RectangleClass::RectangleClass()
{
	left = 0.0;
	top = 0.0;
	right = 0.0;
	bottom = 0.0;
}

RectangleClass::RectangleClass(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

RectangleClass::~RectangleClass()
{

}
