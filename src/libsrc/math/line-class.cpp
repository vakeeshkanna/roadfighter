#include "line-class.h"

LineClass::LineClass()
{
	p1.x = 0.0;
	p1.y = 0.0;
	p2.x = 0.0;
	p2.y = 0.0;
}


LineClass::LineClass(Point myP1, Point myP2)
{
	p1.x = myP1.x;
	p1.y = myP1.y;
	p2.x = myP2.x;
	p2.y = myP2.y;
}

LineClass::LineClass(double myX1, double myY1, double myX2, double myY2)
{
	p1.x = myX1;
	p1.y = myY1;
	p2.x = myX2;
	p2.y = myY2;
}

LineClass::LineClass(const LineClass &other)
{
	p1.x = other.p1.x;
	p1.y = other.p1.y;
	p2.x = other.p2.x;
	p2.y = other.p2.y;
}

LineClass::~LineClass()
{

}

void LineClass::setP1(Point p)
{
	p1.x = p.x;
	p1.x = p.x;
}

void LineClass::setP1(double x1, double y1)
{
	p1.x = x1;
	p1.y = y1;
}

Point LineClass::getP1()
{
	return p1;
}

void LineClass::setP2(Point p)
{
	p2.x = p.x;
	p2.y = p.y;
}

void LineClass::setP2(double x2, double y2)
{
	p2.x = x2;
	p2.y = y2;
}

Point LineClass::getP2()
{
	return p2;
}

double LineClass::length()
{
	double distAB;
	LineClass self(*this);

	self.p2.x -= self.p1.x;
	self.p2.y -= self.p1.y;
	//  Discover the length of segment A-B.
	distAB = sqrt(self.p2.x * self.p2.x + self.p2.y * self.p2.y);

	return distAB;
}

Logical LineClass::intersects(const LineClass &other)
{
	double Ax, Bx, Cx, Dx;
	double Ay, By, Cy, Dy;
	double  distAB, theCos, theSin, newX, ABpos ;

	LineClass lhs(*this);
	LineClass rhs(other);

	Ax = lhs.p1.x;
	Bx = lhs.p2.x;
	Cx = rhs.p1.x;
	Dx = rhs.p2.x;

	Ay = lhs.p1.y;
	By = lhs.p2.y;
	Cy = rhs.p1.y;
	Dy = rhs.p2.y;

	//  Fail if either line segment is zero-length.
	if (Ax == Bx && Ay == By || Cx == Dx && Cy == Dy)
	{
		return no;
	}

	//  Fail if the segments share an end-point.
//	if (Ax == Cx && Ay == Cy || Bx == Cx && By == Cy || Ax == Dx && Ay == Dy || Bx == Dx && By == Dy)
//	{
//		return no;
//	}

	// check if the lines are colinear
	//vertical line
	if(Ax == Bx && Cx == Dx && Ax == Cx)
	{
		if(Ax >= Cx && Ax <= Dx && Bx >= Cx && Bx <= Dx)
		{
			if(Ay >= Cy && Ay <= Dy || By >= Cy && By <= Dy)
			{
				lprintf("vertical case\n");
				return yes;
			}
		}
	}

	//horizontal line
	if(Ay == By && Cy == Dy && Ay == Cy)
	{
		if(Ay >= Cy && Ay <= Dy && By >= Cy && By <= Dy)
		{
			if(Ax >= Cx && Ax <= Dx || Bx >= Cx && Bx <= Dx)
			{
				lprintf("horizontal case\n");
				return yes;
			}
		}
	}

	//  (1) Translate the system so that point A is on the origin.
	Bx -= Ax; By -= Ay;
	Cx -= Ax; Cy -= Ay;
	Dx -=Ax; Dy -=Ay;

	//  Discover the length of segment A-B.
	distAB = sqrt(Bx * Bx+ By *By);

	//  (2) Rotate the system so that point B is on the positive X axis.
	theCos = Bx / distAB;
	theSin = By / distAB;
	newX = Cx * theCos + Cy * theSin;
	Cy = Cy * theCos - Cx * theSin;
	Cx = newX;
	newX = Dx * theCos +Dy * theSin;
	Dy =Dy * theCos - Dx * theSin;
	Dx = newX;

	//  Fail if segment C-D doesn't cross line A-B.
	if (Cy < 0. && Dy < 0. || Cy >= 0. && Dy >= 0.)
		return no;

	//  (3) Discover the position of the intersection point along line A-B.
	ABpos = Dx + (Cx - Dx) * Dy /(Dy - Cy);

	//  Fail if segment C-D crosses line A-B outside of segment A-B.
	if (ABpos < 0. || ABpos > distAB)
		return no;

	//  (4) Apply the discovered position to line A-B in the original coordinate system.
	//*X = Ax + ABpos * theCos;
	//*Y = Ay + ABpos * theSin;

	//  Success.
	return yes;
  }

/*
 * source for intersection of line with rectangle
 * http://sebleedelisle.com/2009/05/super-fast-trianglerectangle-intersection-test/
 */

Logical LineClass::intersectsRect(const RectangleClass rect)
{
	double l,r,t,b;
	double top_intersection;
	double bottom_intersection;
	double toptrianglepoint;
	double bottomtrianglepoint;
	double m;
	double c;
	double x0,x1;
	double y0,y1;


	l = rect.left;
	r = rect.right;
	t = rect.top;
	b = rect.bottom;

	Point start = getP1(), end = getP2();

	x0 = start.getX();
	y0 = start.getY();
	x1 = end.getX();
	y1 = end.getY();

	//case where x0 == x1 or y0 == y1
	if(x0 == x1 || y0 == y1)
	{
		//test individual side of the object in question
		LineClass top(rect.left, rect.top, rect.right, rect.top);
		if(intersects(top))
		{
			return yes;
		}

		LineClass bottom(rect.left, rect.bottom, rect.right, rect.bottom);
		if(intersects(bottom))
		{
			return yes;
		}

		/*LineClass left(rect.left, rect.top, rect.left, rect.bottom);
		if(intersects(left))
		{
			return yes;
		}

		LineClass right(rect.right, rect.top, rect.right, rect.bottom);
		if(intersects(right))
		{
			return yes;
		}*/
		return no;
	}

	// Calculate m and c for the equation for the line (y = mx+c)
	m = (y1 - y0) / (x1 - x0);
	c = y0 - (m * x0);

	// if the line is going up from right to left then the top intersect point is on the left
	if(m>0)
	{
		top_intersection = (m * l  + c);
		bottom_intersection = (m * r  + c);
	}
	// otherwise it's on the right
	else
	{
		top_intersection = (m * r  + c);
		bottom_intersection = (m * l  + c);
	}

	// work out the top and bottom extents for the triangle
	if(y0<y1)
	{
		toptrianglepoint = y0;
		bottomtrianglepoint = y1;
	}
	else
	{
		toptrianglepoint = y1;
		bottomtrianglepoint = y0;
	}

	double topoverlap;
	double botoverlap;

	// and calculate the overlap between those two bounds
	topoverlap = top_intersection > toptrianglepoint ? top_intersection : toptrianglepoint;
	botoverlap = bottom_intersection < bottomtrianglepoint ? bottom_intersection : bottomtrianglepoint;

	// (topoverlap<botoverlap) :
	// if the intersection isn't the right way up then we have no overlap

	// (!((botoverlap<t) || (topoverlap>b)) :
	// If the bottom overlap is higher than the top of the rectangle or the top overlap is
	// lower than the bottom of the rectangle we don't have intersection. So return the negative
	// of that. Much faster than checking each of the points is within the bounds of the rectangle.
	return (topoverlap < botoverlap) && (!((botoverlap < t) || (topoverlap >b )));
}
