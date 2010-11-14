MACRO (USE_MSDEV TARGET)
    TARGET_LINK_LIBRARIES( ${TARGET} ${MSDEV_LIB} )
ENDMACRO(USE_MSDEV)

MACRO(USE_DIRECTX TARGET)
    INCLUDE_DIRECTORIES ( ${DIRECTX9}/Include )
    LINK_DIRECTORIES ( ${DIRECTX9}/Lib/x86 )
    TARGET_LINK_LIBRARIES( ${TARGET} ddraw dsound dxguid)
ENDMACRO(USE_DIRECTX)

MACRO(USE_COMMON TARGET)
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/src/libsrc/common )
    TARGET_LINK_LIBRARIES( ${TARGET} common )
ENDMACRO(USE_COMMON)

MACRO(USE_GAME TARGET)
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/src/libsrc/game )
    TARGET_LINK_LIBRARIES( ${TARGET} game )
ENDMACRO(USE_GAME)

MACRO(USE_RENDERING TARGET)
    USE_DIRECTX( ${TARGET} )
    USE_COMMON(${TARGET})
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/src/libsrc/rendering )
    TARGET_LINK_LIBRARIES( ${TARGET} rendering )
ENDMACRO(USE_RENDERING)

MACRO(USE_MATH TARGET)
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/src/libsrc/math )
    TARGET_LINK_LIBRARIES( ${TARGET} math )
ENDMACRO(USE_MATH)

MACRO(USE_BOOST TARGET)
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/boost/1.36.0/include)
ENDMACRO(USE_BOOST)

MACRO(USE_CPPUNIT TARGET)
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/cppunit/1.12.0/include )
	TARGET_LINK_LIBRARIES( ${TARGET} cppunitd_dll )
ENDMACRO(USE_CPPUNIT)

MACRO(USE_MAPPY TARGET)
	USE_DIRECTX( ${TARGET} )
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/mappy/R_12/include )
	TARGET_LINK_LIBRARIES( ${TARGET} mappy )
ENDMACRO(USE_MAPPY)

MACRO(USE_LUA TARGET)
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/lua/5.14/include )
	LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/lua/5.14/lib )
	TARGET_LINK_LIBRARIES( ${TARGET} lua51 lua5.1 )
ENDMACRO(USE_LUA)

MACRO(USE_PROJ TARGET)
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/proj/4.7.0/include )
	#LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/proj/4.7.0/lib )
	TARGET_LINK_LIBRARIES( ${TARGET} proj_i  )
ENDMACRO(USE_PROJ)

MACRO(USE_SOUND TARGET)
    USE_DIRECTX( ${TARGET} )
    USE_OGG( ${TARGET} )
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/src/libsrc/sound )
    LINK_DIRECTORIES ( ${BUILD_SOURCE_DIR}/src/libsrc/sound )
    TARGET_LINK_LIBRARIES( ${TARGET} sound )
ENDMACRO(USE_SOUND)

MACRO(USE_OGG TARGET)
    USE_DIRECTX( ${TARGET} )
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/ogg/1.2.1/include )
    INCLUDE_DIRECTORIES ( ${BUILD_SOURCE_DIR}/prebuild/vorbis/1.3.2/include )
    TARGET_LINK_LIBRARIES( ${TARGET} libogg )
    TARGET_LINK_LIBRARIES( ${TARGET} libvorbis )
    TARGET_LINK_LIBRARIES( ${TARGET} libvorbisfile )
ENDMACRO(USE_OGG)

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)
