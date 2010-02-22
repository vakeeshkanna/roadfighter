LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )
LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/cppunit/1.12.0/lib/WIN32 )
ADD_EXECUTABLE ( cppunittest cppunittest.cpp string-class-test.cpp point-class-test.cpp line-class-test.cpp)

TARGET_LINK_LIBRARIES ( cppunittest common cppunitd_dll)
USE_COMMON(cppunittest)
USE_GAME(cppunittest)
USE_CPPUNIT(cppunittest)
USE_MATH(cppunittest)