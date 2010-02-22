LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )
ADD_LIBRARY (mappy SHARED mappydll.cpp )
USE_DIRECTX( mappy )
