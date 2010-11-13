LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )
LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/ogg/1.2.1/lib )
LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/vorbis/1.3.2/lib )

ADD_LIBRARY (sound SHARED soundAll)

USE_COMMON(sound)
USE_DIRECTX(sound)
USE_OGG(sound)

install(TARGETS sound RUNTIME DESTINATION bin COMPONENT applications)