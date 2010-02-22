#ifndef CPP_UNIT_LINECLASSTESTCASE_H
#define CPP_UNIT_LINECLASSTESTCASE_H
#include <cppunit/extensions/HelperMacros.h>

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class LineClassTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LineClassTestCase );
  CPPUNIT_TEST( testDiagonalCase );
  CPPUNIT_TEST( testVerticalCase );
  CPPUNIT_TEST( testHorizontalCase );
  CPPUNIT_TEST( testLinesNotIntersects );
  //CPPUNIT_TEST( testCopy );
  //CPPUNIT_TEST( testTrim );
  //CPPUNIT_TEST( testComparison );
  //CPPUNIT_TEST( testSubstring );
  //CPPUNIT_TEST( testMisc );
  //CPPUNIT_TEST( testStress );
  CPPUNIT_TEST_SUITE_END();

 protected:

 public:
  void setUp();

 protected:
  void testDiagonalCase();
  void testVerticalCase();
  void testHorizontalCase();
  void testLinesNotIntersects();
};

#endif
