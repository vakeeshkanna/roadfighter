cmake_minimum_required(VERSION 2.6)

project (BUILD)

IF (WIN32)
ADD_DEFINITIONS ( -D_CRT_SECURE_NO_WARNINGS )
ADD_DEFINITIONS ( -DWIN32 )
ADD_DEFINITIONS ( "-DEXPORT=__declspec(dllexport)" )
ELSE (WIN32)
ADD_DEFINITIONS ( -DLINUX )
ADD_DEFINITIONS ( "-DEXPORT=" )
ENDIF (WIN32)

IF (WIN32)
SET ( DIRECTX9 "D:/apps/DirectX_Mar09" )
SET ( MSDEV_LIB kernel32 user32 gdi32 winspool comdlg32 winmm advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32 )
SET ( ROOT_DIR "D:/mercurial/source_cmake_2.6" )
SET ( RESOURCES_DIR "${ROOT_DIR}/resources" )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/bin )
ENDIF (WIN32)

INCLUDE("modules.cmake")

ADD_SUBDIRECTORY ( src )
ADD_SUBDIRECTORY ( unittest/cppunittest )

MESSAGE( "Copying resources" )
configure_files ( "${RESOURCES_DIR}/maps" "${ROOT_DIR}/build/bin/debug")
configure_files ( "${RESOURCES_DIR}/images" "${ROOT_DIR}/build/bin/debug")
configure_files ( "${RESOURCES_DIR}/sounds" "${ROOT_DIR}/build/bin/debug")
MESSAGE( "Copying resources done" )

#install(FILES  ${RESOURCES_DIR}/maps/stage1.FMP DESTINATION ${EXECUTABLE_OUTPUT_PATH})
#install(DIRECTORY ${RESOURCES_DIR}/maps/ DESTINATION ${EXECUTABLE_OUTPUT_PATH}
#          FILES_MATCHING PATTERN "*.FMP")

#install(TARGETS roadfighter common demo math sound rendering 
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION bin)
        #ARCHIVE DESTINATION lib/static)
