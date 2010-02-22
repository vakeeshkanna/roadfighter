#ifndef CPP_UNIT_STRINGTESTCASE_H
#define CPP_UNIT_STRINGTESTCASE_H
#include <string>
#include <cppunit/extensions/HelperMacros.h>

#include "string-class.h"

/* 
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class StringTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( StringTestCase );
  CPPUNIT_TEST( testCreation );
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
  void testCreation();
 // void testCopy();
 // void testTrim();
  //void testComparison();
 // void testSubstring();
 // void testMisc();
  //void testStress();
  };

#endif