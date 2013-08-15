cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)


project (AppKit_Glut)


set (SOURCEFILES 
source/MVRApp_Glut.cpp
source/MVRWindow_Glut.cpp
)


set (HEADERFILES
include/MVRGlut/MVRApp_Glut.H
include/MVRGlut/MVRWindow_Glut.h 
)
source_group("Header Files" FILES ${HEADERFILES})


# Include Directories

include_directories (
  .
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/G3DLite/include
  ${CMAKE_SOURCE_DIR}/MVRCore/include
)


# Build Target

add_library ( ${PROJECT_NAME} ${HEADERFILES} ${SOURCEFILES} )
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "App Kits")


