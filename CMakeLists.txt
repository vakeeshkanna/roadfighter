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
SET ( PREBUILD_DIR "${ROOT_DIR}/prebuild" )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/bin )
ENDIF (WIN32)

INCLUDE("modules.cmake")

ADD_SUBDIRECTORY ( src )
ADD_SUBDIRECTORY ( unittest/cppunittest )

MESSAGE( "Copying images, maps and sounds" )
configure_files ( "${RESOURCES_DIR}/maps" "${ROOT_DIR}/build/bin/maps")
configure_files ( "${RESOURCES_DIR}/images" "${ROOT_DIR}/build/bin/images")
configure_files ( "${RESOURCES_DIR}/sounds" "${ROOT_DIR}/build/bin/sounds")

MESSAGE( "Copying libogg.dll" )
configure_files ( "${PREBUILD_DIR}/ogg/1.2.1/lib/Debug" "${ROOT_DIR}/build/bin/Debug")
configure_files ( "${PREBUILD_DIR}/ogg/1.2.1/lib/Release" "${ROOT_DIR}/bin/Release")

MESSAGE( "Copying libvorbis.dll and libvorbisfile.dll" )
configure_files ( "${PREBUILD_DIR}/vorbis/1.3.2/lib/Debug" "${ROOT_DIR}/build/bin/Debug")
configure_files ( "${PREBUILD_DIR}/vorbis/1.3.2/lib/Release" "${ROOT_DIR}/build/bin/Release")

MESSAGE( "Copying resources done" )

IF(MSVC90)
	FIND_PROGRAM(MSVC_REDIST NAMES
	redist/vcredist_${CMAKE_MSVC_ARCH}.exe PATHS
	"{ROOT_DIR}/resources")
    GET_FILENAME_COMPONENT(vcredist_name "${MSVC_REDIST}" NAME)
    INSTALL(PROGRAMS ${MSVC_REDIST} COMPONENT Runtime DESTINATION bin)
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\${vcredist_name}\\\"'")
	MESSAGE( "vc redist copied" )
ENDIF(MSVC90)

IF(MSVC90)
	FIND_PROGRAM(DIRECTX_REDIST NAMES
	redist/directx_9c_redist.exe PATHS
	"{ROOT_DIR}/resources")
    GET_FILENAME_COMPONENT(directx_redist_name "${DIRECTX_REDIST}" NAME)
    INSTALL(PROGRAMS ${DIRECTX_REDIST} COMPONENT Runtime DESTINATION bin)
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\${directx_redist_name}\\\"'")
	MESSAGE( "directx redist copied" )
ENDIF(MSVC90)

IF(MSVC90)
	FIND_PROGRAM(OGG_REDIST NAMES "ogg/1.2.1/lib/Release/libogg.dll" PATHS "${PREBUILD_DIR}")
    GET_FILENAME_COMPONENT(oggredist_name "${OGG_REDIST}" NAME)
    INSTALL(PROGRAMS ${OGG_REDIST} COMPONENT Runtime DESTINATION bin)
	MESSAGE( "ogg redist copied" )
ENDIF(MSVC90)

IF(MSVC90)
	FIND_PROGRAM(VORBIS_REDIST NAMES "vorbis/1.3.2/lib/Release/libvorbis.dll" PATHS "${PREBUILD_DIR}")
    GET_FILENAME_COMPONENT(vorbisredist_name "${VORBIS_REDIST}" NAME)
    INSTALL(PROGRAMS ${VORBIS_REDIST} COMPONENT Runtime DESTINATION bin)
	MESSAGE( "vorbis redist copied" )
ENDIF(MSVC90)

IF(MSVC90)
	FIND_PROGRAM(VORBISFILE_REDIST NAMES "vorbis/1.3.2/lib/Release/libvorbisfile.dll" PATHS "${PREBUILD_DIR}")
    GET_FILENAME_COMPONENT(vorbisfileredist_name "${VORBISFILE_REDIST}" NAME)
    INSTALL(PROGRAMS ${VORBISFILE_REDIST} COMPONENT Runtime DESTINATION bin)
	MESSAGE( "vorbisfile redist copied" )
ENDIF(MSVC90)

configure_file( "${ROOT_DIR}/Roadfighter-1.0.0-win32.exe" "${ROOT_DIR}/build/installers" COPYONLY)
#configure_files ( "${ROOT_DIR}/Roadfighter-1.0.0-win32.exe" "${ROOT_DIR}/build/installers")

INCLUDE(CPack)