#ifndef POINT_CLASS_H_
#define POINT_CLASS_H_

class EXPORT Point
{
public:
	Point();
	Point(double x,double y);
	Point(double x,double y,double z);
	~Point();
	void setX(double x);
	double getX();
	void setY(double y);
	double getY();
	void setZ(double z);
	double getZ();
	void setXY(double x,double y);
	void setXYZ(double x,double y,double z);
	Point& operator=(const Point& src);
	Point& operator+(const Point& src);
	Point& operator-(const Point& src);

	double x,y,z;
};

#endif /*POINT_CLASS_H_*/
