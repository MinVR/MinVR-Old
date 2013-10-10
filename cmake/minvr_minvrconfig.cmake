# Configures the MinVRConfig.cmake and MinVRConfigVersion.cmake files
#

# Handle MVRCore dependencies
set(MVRCONFIG_AVAILABLE_COMPONENTS "MVRCore")
set(MVRCONFIG_AVAILABLE_COMPONENTS_LIST "# - MVRCore")
set(MVRCONFIG_DEPENDENCIES "set(minvr_MVRCore_dep Boost")
if (USE_VRPN)
	set(MVRCONFIG_DEPENDENCIES "${MVRCONFIG_DEPENDENCIES} LibVRPN")
endif(USE_VRPN)
if (USE_TUIO)
	set(MVRCONFIG_DEPENDENCIES "${MVRCONFIG_DEPENDENCIES} LibTUIO")
endif(USE_TUIO)
if (USE_SPACENAV)
	set(MVRCONFIG_DEPENDENCIES "${MVRCONFIG_DEPENDENCIES} LibSpaceNav")
endif(USE_SPACENAV)
set(MVRCONFIG_DEPENDENCIES "${MVRCONFIG_DEPENDENCIES})\n")

#--------------------------------------
# Handle AppKit components and dependencies
#--------------------------------------
if (USE_APPKIT_GLFW)
	set(MVRCONFIG_AVAILABLE_COMPONENTS "${MVRCONFIG_AVAILABLE_COMPONENTS} AppKit_GLFW")
	set(MVRCONFIG_AVAILABLE_COMPONENTS_LIST "${MVRCONFIG_AVAILABLE_COMPONENTS_LIST}\n# - AppKit_GLFW")
	set(MVRCONFIG_DEPENDENCIES "${MVRCONFIG_DEPENDENCIES}set(minvr_AppKit_GLFW_dep glfw)\n")
endif(USE_APPKIT_GLFW)

if (USE_APPKIT_G3D9)
        set(MVRCONFIG_AVAILABLE_COMPONENTS "${MVRCONFIG_AVAILABLE_COMPONENTS} AppKit_G3D9")
        set(MVRCONFIG_AVAILABLE_COMPONENTS_LIST "${MVRCONFIG_AVAILABLE_COMPONENTS_LIST}\n# - AppKit_G3D9")
        set(MVRCONFIG_DEPENDENCIES "${MVRCONFIG_DEPENDENCIES}set(minvr_AppKit_G3D9_dep G3D9)\n")
endif(USE_APPKIT_G3D9)

if (USE_APPKIT_GLUT)
	message("Appkit glut not implemented yet")
endif(USE_APPKIT_GLUT)

configure_file("${CMAKE_SOURCE_DIR}/cmake/MinVRConfig.cmake.in"
               "${CMAKE_BINARY_DIR}/MinVRConfig.cmake" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/cmake/MinVRConfigVersion.cmake.in"
               "${CMAKE_BINARY_DIR}/MinVRConfigVersion.cmake" @ONLY)
install(FILES
        "${CMAKE_BINARY_DIR}/MinVRConfig.cmake"
        "${CMAKE_BINARY_DIR}/MinVRConfigVersion.cmake"
        DESTINATION "${MINVR_INSTALL_DIR}")