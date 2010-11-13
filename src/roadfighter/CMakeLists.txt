LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )
LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/ogg/1.2.1/lib )
LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/vorbis/1.3.2/lib )

ADD_EXECUTABLE ( roadfighter roadfighterAll.cpp )

USE_MSDEV(roadfighter)
USE_DIRECTX(roadfighter)
USE_RENDERING(roadfighter)
USE_SOUND(roadfighter)
USE_GAME(roadfighter)
USE_MATH(roadfighter)
USE_MAPPY(roadfighter)

SET_TARGET_PROPERTIES ( roadfighter PROPERTIES LINK_FLAGS "-subsystem:windows" )

install(TARGETS roadfighter 
        RUNTIME 
		DESTINATION bin
		COMPONENT applications)

install(DIRECTORY  ${RESOURCES_DIR}/maps
		DESTINATION .
		COMPONENT resources
		FILES_MATCHING PATTERN "*.fmp")

install(DIRECTORY  ${RESOURCES_DIR}/images
		DESTINATION .
		COMPONENT resources
		FILES_MATCHING PATTERN "*.bmp")

install(DIRECTORY  ${RESOURCES_DIR}/sounds
		DESTINATION .
		COMPONENT resources
		FILES_MATCHING PATTERN "*.wav")