#include "line-class-test.h"
#include "basedefs.h"
#include <float.h>
#include <math.h>
#include "log-class.h"
#include "line-class.h"
#include "helper-class.h"
CPPUNIT_TEST_SUITE_REGISTRATION( LineClassTestCase );

void LineClassTestCase::setUp()
{
}

void LineClassTestCase::testDiagonalCase()
{
	LineClass lhs(1.0, 1.0, 4.0, 4.0);
	LineClass rhs(3.0, 1.0, 3.0, 4.0);
	Logical intersection = lhs.intersects(rhs);
	CPPUNIT_ASSERT(intersection == yes);
}

void LineClassTestCase::testVerticalCase()
{
	LineClass lhs(3.0, 1.0, 3.0, 4.0);
	LineClass rhs(3.0, 2.0, 3.0, 6.0);
	Logical intersection = lhs.intersects(rhs);
	CPPUNIT_ASSERT(intersection == yes);
}

void LineClassTestCase::testHorizontalCase()
{
	LineClass lhs(1.0, 3.0, 4.0, 3.0);
	LineClass rhs(2.0, 3.0, 6.0, 3.0);
	Logical intersection = lhs.intersects(rhs);
	CPPUNIT_ASSERT(intersection == yes);
}

void LineClassTestCase::testLinesNotIntersects()
{
	LineClass lhs(1.0, 1.0, 4.0, 4.0);
	LineClass rhs(4.0, 3.0, 5.0, 1.0);
	Logical intersection = lhs.intersects(rhs);
	CPPUNIT_ASSERT(intersection == no);
}
