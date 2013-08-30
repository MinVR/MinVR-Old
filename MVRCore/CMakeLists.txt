cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)


project (MVRCore)


set (SOURCEFILES
source/AbstractMVREngine.cpp
source/AbstractWindow.cpp
source/CameraOffAxis.cpp
source/ConfigMap.cpp
source/ConfigVal.cpp
source/DataFileUtils.cpp
source/Event.cpp
source/InputDeviceSpaceNav.cpp
source/InputDeviceTUIOClient.cpp
source/InputDeviceVRPNAnalog.cpp
source/InputDeviceVRPNButton.cpp
source/InputDeviceVRPNTracker.cpp
source/RenderThread.cpp
source/StringUtils.cpp
source/Rect2D.cpp
)

set (HEADERFILES
include/MVRCore/AbstractCamera.H
include/MVRCore/AbstractInputDevice.H
include/MVRCore/AbstractMVRApp.H
include/MVRCore/AbstractMVREngine.H
include/MVRCore/AbstractWindow.H
include/MVRCore/CameraOffAxis.H
include/MVRCore/CameraTraditional.H
include/MVRCore/ConfigMap.H
include/MVRCore/ConfigVal.H
include/MVRCore/DataFileUtils.H
include/MVRCore/Event.H
include/MVRCore/InputDeviceSpaceNav.H
include/MVRCore/InputDeviceTUIOClient.H
include/MVRCore/InputDeviceVRPNAnalog.H
include/MVRCore/InputDeviceVRPNButton.H
include/MVRCore/InputDeviceVRPNTracker.H
include/MVRCore/RenderThread.H
include/MVRCore/StringUtils.H
include/MVRCore/WindowSettings.H
include/MVRCore/Rect2D.H
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	# Mac OS X specific code
	set (SOURCEFILES ${SOURCEFILES} source/InputDeviceSpaceNav_OSX.cpp)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# Linux specific code
	set (SOURCEFILES ${SOURCEFILES} source/InputDeviceSpaceNav_Linux.cpp)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set (SOURCEFILES ${SOURCEFILES} source/InputDeviceSpaceNav_Windows.cpp)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

source_group("Header Files" FILES ${HEADERFILES})


# Include Directories

include_directories (
  .
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/dependencies/glm
)

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
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBS_ALL} )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION "${MINVR_INSTALL_DIR}/include")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/shaders/ DESTINATION "${MINVR_INSTALL_DIR}/share/shaders")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/vrsetup/ DESTINATION "${MINVR_INSTALL_DIR}/share/vrsetup")

add_dependencies(${PROJECT_NAME} boost)

