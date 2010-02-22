#include "string-class-test.h"
#include <float.h>
#include <math.h>
#include "log-class.h"
CPPUNIT_TEST_SUITE_REGISTRATION( StringTestCase );

void StringTestCase::setUp()
{
}

void StringTestCase::testCreation()
{
  String str = "abc";
  try 
  {
	  CPPUNIT_ASSERT ( strcmp(str.str(),"abc") == 0 );
  } 
  catch (...) 
  {
    CPPUNIT_FAIL("Exception thrown while creating dfastr object");
  }
}
