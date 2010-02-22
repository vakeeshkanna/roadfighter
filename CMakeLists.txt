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
SET ( DIRECTX9 "F:/devtools/DirectX9.0" )
SET ( MSDEV_LIB kernel32 user32 gdi32 winspool comdlg32 winmm advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32 )
SET ( OUTPUT_BIN_DIR "C:/dev/bin" )
ENDIF (WIN32)

INCLUDE("modules.cmake")

ADD_SUBDIRECTORY ( src )
ADD_SUBDIRECTORY ( unittest/cppunittest ${OUTPUT_BIN_DIR} )
