LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )

ADD_LIBRARY (sound SHARED soundAll)

USE_COMMON(sound)
USE_DIRECTX(sound)

