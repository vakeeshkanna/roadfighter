#include "point-class.h"

using namespace std;
Point::Point()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Point::Point(double newX,double newY)
{
	x = newX;
	y = newY;
}

Point::Point(double newX,double newY,double newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

void Point::setX(double newX)
{
	x = newX;
}

double Point::getX()
{
	return x;
}

void Point::setY(double newY)
{
	y = newY;
}

double Point::getY()
{
	return y;
}

void Point::setZ(double newZ)
{
	z = newZ;
}

double Point::getZ()
{
	return z;
}

void Point::setXY(double newX,double newY)
{
	x = newX;
	y = newY;
}

void Point::setXYZ(double newX,double newY,double newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

Point& Point::operator=(const Point& lhs)
{
	x = lhs.x;
	y = lhs.y;
	z = lhs.z;
	return *this;
}

Point& Point::operator+(const Point& lhs)
{
	x += lhs.x;
	y += lhs.y;
	z += lhs.z;
	return *this;
}

Point& Point::operator-(const Point& lhs)
{
	x -= lhs.x;
	y -= lhs.y;
	z -= lhs.z;
	return *this;
}

Point::~Point()
{
}
