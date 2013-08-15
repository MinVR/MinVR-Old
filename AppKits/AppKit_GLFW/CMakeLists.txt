cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)

project (AppKit_GLFW)

set (SOURCEFILES 
source/MVREngineGLFW.cpp
source/WindowGLFW.cpp
source/glew.c
)

set (HEADERFILES
include/AppKit_GLFW/MVREngineGLFW.H
include/AppKit_GLFW/WindowGLFW.H
include/GL/glew.h
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set (HEADERFILES ${HEADERFILES} include/GL/wglew.h)
else ()
	set (HEADERFILES ${HEADERFILES} include/GL/glxew.h)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

add_definitions(-DGLEW_STATIC)

source_group("Header Files" FILES ${HEADERFILES})

set(glfw_checkout_Dir ${CMAKE_SOURCE_DIR}/dependencies/glfw)
make_directory(${glfw_checkout_Dir})

include(ExternalProject)
set(glfw_PREFIX "${glfw_checkout_Dir}")
set(glfw_INSTALL_DIR "${MINVR_INSTALL_DIR}/glfw")
set(glfw_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glfw_INSTALL_DIR})
set(glfw_DIR "${glfw_INSTALL_DIR}")

ExternalProject_add(glfw
		PREFIX ${glfw_PREFIX}
		GIT_REPOSITORY https://github.com/bretjackson/glfw.git
		GIT_TAG gpu-affinity
		INSTALL_DIR ${glfw_INSTALL_DIR}
		CMAKE_ARGS ${glfw_CMAKE_ARGS}
 )
 
set_property(TARGET glfw PROPERTY FOLDER "Dependencies")

set(GLFW_INCLUDE_DIR ${glfw_INSTALL_DIR}/include CACHE INTERNAL "Directory of GLFW header files")
set(GLFW_LIBRARY ${glfw_INSTALL_DIR}/lib/glfw3.lib CACHE INTERNAL "GLFW lib file")

include_directories(${GLFW_INCLUDE_DIR})

# Include Directories

include_directories (
  .
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/dependencies/G3DLite/include
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
add_dependencies(${PROJECT_NAME} boost glfw G3DLite MVRCore)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION "${MINVR_INSTALL_DIR}/include")

