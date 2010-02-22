LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )

ADD_LIBRARY (rendering SHARED rendering )

USE_COMMON(rendering)
USE_DIRECTX(rendering)

