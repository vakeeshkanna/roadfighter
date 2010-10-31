cmake_minimum_required(VERSION 2.6)

project (BUILD)

IF(CMAKE_CL_64)
     SET(CMAKE_MSVC_ARCH amd64)
   ELSE(CMAKE_CL_64)
     SET(CMAKE_MSVC_ARCH x86)
ENDIF(CMAKE_CL_64)


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
SET ( ROOT_DIR "D:/mercurial/source" )
SET ( RESOURCES_DIR "${ROOT_DIR}/resources" )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/bin )
ENDIF (WIN32)

INCLUDE("modules.cmake")

ADD_SUBDIRECTORY ( src )
ADD_SUBDIRECTORY ( unittest/cppunittest )

MESSAGE( "Copying resources" )
configure_files ( "${RESOURCES_DIR}/maps" "${ROOT_DIR}/build/bin/maps")
configure_files ( "${RESOURCES_DIR}/images" "${ROOT_DIR}/build/bin/images")
configure_files ( "${RESOURCES_DIR}/sounds" "${ROOT_DIR}/build/bin/sounds")
MESSAGE( "Copying resources done" )

IF(MSVC90)
	FIND_PROGRAM(MSVC_REDIST NAMES
	redist/vcredist_${CMAKE_MSVC_ARCH}.exe PATHS
	"D:/mercurial/source/resources")
    GET_FILENAME_COMPONENT(vcredist_name "${MSVC_REDIST}" NAME)
    INSTALL(PROGRAMS ${MSVC_REDIST} COMPONENT Runtime DESTINATION bin)
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\${vcredist_name}\\\"'")
	MESSAGE( "VC FOUND" )
ENDIF(MSVC90)

IF(MSVC90)
	FIND_PROGRAM(DIRECTX_REDIST NAMES
	redist/directx_9c_redist.exe PATHS
	"D:/mercurial/source/resources")
    GET_FILENAME_COMPONENT(directx_redist_name "${DIRECTX_REDIST}" NAME)
    INSTALL(PROGRAMS ${DIRECTX_REDIST} COMPONENT Runtime DESTINATION bin)
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\${directx_redist_name}\\\"'")
	MESSAGE( "VC FOUND" )
ENDIF(MSVC90)

configure_file( "${ROOT_DIR}/Roadfighter-1.0.0-win32.exe" "${ROOT_DIR}/build/installers" COPYONLY)
#configure_files ( "${ROOT_DIR}/Roadfighter-1.0.0-win32.exe" "${ROOT_DIR}/build/installers")

INCLUDE(CPack)