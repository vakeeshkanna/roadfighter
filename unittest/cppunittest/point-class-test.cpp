#include "point-class-test.h"
CPPUNIT_TEST_SUITE_REGISTRATION( PointTestCase );

void PointTestCase::setUp()
{
}

void PointTestCase::testCreation()
{
  Point p(1,2,3);
  try
  {
	  CPPUNIT_ASSERT (p.getX() == 1.0);
	  CPPUNIT_ASSERT (p.getY() == 2.0);
	  CPPUNIT_ASSERT (p.getZ() == 3.0);
  }
  catch (...)
  {
    CPPUNIT_FAIL("Exception thrown while creating point object");
  }
}

void PointTestCase::testEqualOperator()
{
	Point p1(1,2,3),p2;
	p2 = p1;
	CPPUNIT_ASSERT (p1.getX() == p2.getX());
    CPPUNIT_ASSERT (p1.getY() == p2.getY());
    CPPUNIT_ASSERT (p1.getZ() == p2.getZ());
}

void PointTestCase::testPlusOperator()
{
	Point p1(1,2,3),p2(1,2,3);
	p1 = p1 + p2;
	CPPUNIT_ASSERT (p1.getX() == 2.0);
    CPPUNIT_ASSERT (p1.getY() == 4.0);
    CPPUNIT_ASSERT (p1.getZ() == 6.0);
}

void PointTestCase::testSubtractOperator()
{
	Point p1(1,2,3),p2(1,2,3);
	p1 = p1 - p2;
	CPPUNIT_ASSERT (p1.getX() == 0.0);
    CPPUNIT_ASSERT (p1.getY() == 0.0);
    CPPUNIT_ASSERT (p1.getZ() == 0.0);
}


