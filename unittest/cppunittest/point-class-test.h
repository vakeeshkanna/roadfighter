#ifndef CPP_UNIT_POINTTESTCASE_H
#define CPP_UNIT_POINTTESTCASE_H
#include <string>
#include <cppunit/extensions/HelperMacros.h>

#include "point-class.h"

/* 
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class PointTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( PointTestCase );
  CPPUNIT_TEST( testCreation );
  CPPUNIT_TEST( testEqualOperator );
  CPPUNIT_TEST( testPlusOperator );
  CPPUNIT_TEST( testSubtractOperator );
  CPPUNIT_TEST_SUITE_END();
  
 protected:
  
 public:
  void setUp();
  
 protected:
  void testCreation();
  void testEqualOperator();
  void testPlusOperator();
  void testSubtractOperator();
  };

#endif