#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <fstream>
#include <iostream>

int
main( int argc, char* argv[] )
{
  // Create the event manager and test controller
  CPPUNIT_NS::TestResult controller;

  // Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener( &result );        

  // Add a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener( &progress );      

  // Add the top suite to the test runner
  CPPUNIT_NS::TestRunner runner;
  runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
  runner.run( controller );

  // Print test in a compiler compatible format.
   CPPUNIT_NS::CompilerOutputter outputter( &result, std::cerr );
   outputter.write(); 

  // Print test in a compiler compatible format.
//  std::ofstream ofs("results.xml");
//  CPPUNIT_NS::XmlOutputter outputter( &result, ofs );
//  outputter.setStyleSheet("report.xsl");
//  outputter.write(); 

  return result.wasSuccessful() ? 0 : 1;
}