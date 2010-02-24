LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )

ADD_EXECUTABLE ( roadfighter roadfighterAll.cpp )

USE_MSDEV(roadfighter)
USE_DIRECTX(roadfighter)
USE_RENDERING(roadfighter)
USE_SOUND(roadfighter)
USE_GAME(roadfighter)
USE_MATH(roadfighter)
USE_MAPPY(roadfighter)

IF (WIN32)
SET_TARGET_PROPERTIES ( roadfighter PROPERTIES LINK_FLAGS "-subsystem:windows" )
ENDIF (WIN32)
