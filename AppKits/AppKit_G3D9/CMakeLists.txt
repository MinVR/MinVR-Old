cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)

project (AppKit_G3D9)

set (SOURCEFILES 
source/MVREngineG3D9.cpp
source/WindowG3D9.cpp
source/OffAxisCameraG3D9.cpp
)

set (HEADERFILES
include/AppKit_G3D9/MVREngineG3D9.H
include/AppKit_G3D9/WindowG3D9.H
include/AppKit_G3D9/OffAxisCameraG3D9.H
)

source_group("Header Files" FILES ${HEADERFILES})

set(g3d9_checkout_Dir ${CMAKE_SOURCE_DIR}/dependencies/G3D9)
make_directory(${g3d9_checkout_Dir})

include(ExternalProject)
set(g3d9_PREFIX "${g3d9_checkout_Dir}")
set(g3d9_INSTALL_DIR "${MINVR_INSTALL_DIR}/G3D9")
if (MSVC)
	#set the permissions to execute
	#set(g3d9_CONFIGURE_COMMAND "icacls <SOURCE_DIR>/buildg3d.bat /grant Everyone:RX; icacls <SOURCE_DIR>/buildg3d /grant Everyone:RX; icacls <SOURCE_DIR>/bin /grant Everyone:(OI)(CI)RX")
	set(g3d9_CONFIGURE_COMMAND  ${CMAKE_COMMAND} -E echo Configuring G3D)
	set(g3d9_BUILD_COMMAND  <SOURCE_DIR>/buildg3d.bat --install ${g3d9_INSTALL_DIR} lib)
else ()
	set(g3d9_CONFIGURE_COMMAND "chmod +x <SOURCE_DIR>/buildg3d; chmod +x <SOURCE_DIR>/bin/*")
	set(g3d9_BUILD_COMMAND  <SOURCE_DIR>/buildg3d --install ${g3d9_INSTALL_DIR} lib)
endif()
set(g3d9_DIR "${g3d9_INSTALL_DIR}")

ExternalProject_add(G3D9
		PREFIX ${g3d9_PREFIX}
		URL http://sourceforge.net/projects/g3d/files/g3d-cpp/9.00/G3D-9.00-src.zip
		URL_MD5 fe0d95eebdc90ef95aca13762372155f
		CONFIGURE_COMMAND ${g3d9_CONFIGURE_COMMAND}
		BUILD_COMMAND ${g3d9_BUILD_COMMAND}
		INSTALL_DIR ${g3d9_INSTALL_DIR}
		BUILD_IN_SOURCE 1
 )
 
set_property(TARGET G3D9 PROPERTY FOLDER "Dependencies")

set(G3D9_INCLUDE_DIRS ${g3d9_INSTALL_DIR}/G3D/include CACHE INTERNAL "Directory of G3D9 header files")
set(G3D9_LIBRARIES "optimized ${g3d9_INSTALL_DIR}/G3D/lib/G3D.lib debug ${g3d9_INSTALL_DIR}/G3D/lib/G3Dd.lib optimized ${g3d9_INSTALL_DIR}/G3D/lib/GLG3D.lib debug ${g3d9_INSTALL_DIR}/lib/GLG3Dd.lib optimized ${g3d9_INSTALL_DIR}/G3D/lib/assimp.lib debug ${g3d9_INSTALL_DIR}/G3D/lib/assimpd.lib ${g3d9_INSTALL_DIR}/G3D/lib/avcodec.lib ${g3d9_INSTALL_DIR}/G3D/lib/avformat.lib ${g3d9_INSTALL_DIR}/G3D/lib/avutil.lib optimized ${g3d9_INSTALL_DIR}/G3D/lib/civetweb.lib debug ${g3d9_INSTALL_DIR}/G3D/lib/civetwebd.lib ${g3d9_INSTALL_DIR}/G3D/lib/enet.lib optimized ${g3d9_INSTALL_DIR}/G3D/lib/freeimage.lib debug ${g3d9_INSTALL_DIR}/G3D/lib/freeimaged.lib ${g3d9_INSTALL_DIR}/G3D/lib/glfw.lib optimized ${g3d9_INSTALL_DIR}/G3D/lib/qrencode.lib debug ${g3d9_INSTALL_DIR}/G3D/lib/qrencoded.lib ${g3d9_INSTALL_DIR}/G3D/lib/swscale.lib ${g3d9_INSTALL_DIR}/G3D/lib/zip.lib ${g3d9_INSTALL_DIR}/G3D/lib/zlib.lib" CACHE INTERNAL "G3D lib files")

include_directories(${G3D9_INCLUDE_DIRS})

# Include Directories

include_directories (
  .
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/dependencies/glm
  ${CMAKE_SOURCE_DIR}/MVRCore/include
)

# Windows Section #
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	# Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

make_directory(${CMAKE_BINARY_DIR}/lib)
make_directory(${CMAKE_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach (CONF ${CMAKE_CONFIGURATION_TYPES})
	string (TOUPPER ${CONF} CONF)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/bin)
	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/lib)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF} ${CMAKE_BINARY_DIR}/lib)
endforeach(CONF CMAKE_CONFIGURATION_TYPES)

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
set(CMAKE_MINSIZEREL_POSTFIX "s")

#set the build postfix extension according to the current configuration
if (CMAKE_BUILD_TYPE MATCHES "Release")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
	set(CMAKE_BUILD_POSTFIX "")
endif()

# Build Target

add_library ( ${PROJECT_NAME} ${HEADERFILES} ${SOURCEFILES} )
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "App Kits")
add_dependencies(${PROJECT_NAME} boost G3D9 MVRCore)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION "${MINVR_INSTALL_DIR}/include")

