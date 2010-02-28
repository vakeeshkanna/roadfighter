ADD_LIBRARY (common SHARED common )
USE_MSDEV(common)

install(TARGETS common RUNTIME DESTINATION bin COMPONENT applications)