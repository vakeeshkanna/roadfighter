LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )
ADD_LIBRARY (mappy SHARED mappydll.cpp )
USE_DIRECTX( mappy )

install(TARGETS mappy RUNTIME DESTINATION . COMPONENT applications)