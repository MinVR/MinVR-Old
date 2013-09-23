cmake_minimum_required (VERSION 2.8)
project (TUIO)

aux_source_directory(${CMAKE_SOURCE_DIR} SOURCEFILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/ip/ SOURCEFILES)
aux_source_directory(${CMAKE_SOURCE_DIR}/osc/ SOURCEFILES)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  aux_source_directory(${CMAKE_SOURCE_DIR}/ip/win32/ SOURCEFILES)
else()
  aux_source_directory(${CMAKE_SOURCE_DIR}/ip/posix/ SOURCEFILES)
endif()  

set (CMAKE_VERBOSE_MAKEFILE TRUE)

# Used for compiling in subdirectories of the project
include_directories (
  .
)


############################################################
#OS Specific flags should go here
############################################################
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++98")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++98")
	add_definitions("-DOSC_HOST_LITTLE_ENDIAN")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

############################################################
#Finally add all the targets here
############################################################

add_library(TUIO ${SOURCEFILES})
