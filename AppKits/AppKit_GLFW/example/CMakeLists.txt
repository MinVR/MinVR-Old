cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)

project (AppKit_GLFW_DemoApp)

set (SOURCEFILES 
source/GLFWDemoApp.cpp
source/main.cpp
../source/glew.c
)

set (HEADERFILES
include/GLFWDemoApp.H
../include/GL/glew.h
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set (HEADERFILES ${HEADERFILES} ../include/GL/wglew.h)
else ()
	set (HEADERFILES ${HEADERFILES} ../include/GL/glxew.h)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

source_group("Header Files" FILES ${HEADERFILES})

# Include Directories
include_directories (
  .
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/../include
  ${CMAKE_SOURCE_DIR}/dependencies/glm
  ${CMAKE_SOURCE_DIR}/MVRCore/include
  ${GLFW_INCLUDE_DIR}
)

link_directories (
  ${AppKit_GLFW_BINARY_DIR}
  ${MVRCore_BINARY_DIR}
)

# Windows Section #
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DGLEW_STATIC)
	# These libraries seem to fix a couple linker errors with TUIO.lib
	if (USE_TUIO)
		set (LIBS_ALL ${LIBS_ALL} ws2_32.lib winmm.lib)
	endif()
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
	set_target_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	find_library(COCOA_LIB Cocoa)
	find_library(IOKIT_LIB IOKit)
	set(LIBS_ALL ${LIBS_ALL} ${COCOA_LIB} ${IOKIT_LIB})
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(Threads)
	find_package(X11)
	set(LIBS_ALL ${LIBS_ALL} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m ${X11_LIBRARIES})
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
add_executable ( ${PROJECT_NAME} ${HEADERFILES} ${SOURCEFILES} )
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Examples")
target_link_libraries(${PROJECT_NAME} AppKit_GLFW MVRCore ${GLFW_LIBRARY} ${Boost_LIBRARIES} ${LIBS_OPT} ${LIBS_DEBUG} ${LIBS_ALL})
add_dependencies( ${PROJECT_NAME} boost MVRCore glfw AppKit_GLFW)

