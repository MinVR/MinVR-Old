find_path(LIBVRPN_INCLUDE_DIR vrpn_Tracker.h
          HINTS $ENV{VRPN_INCLUDE_DIR} ${VRPN_INCLUDE_DIR} $ENV{G}/src/vrpn/7.30/vrpn )

find_library(LIBVRPN_OPT_LIBRARY NAMES libvrpn.a vrpn.lib vrpn
          HINTS $ENV{G}/src/vrpn/7.30/vrpn/build/${GBUILDSTR}/Release ${VRPN_LIB_DIR})

find_library(LIBVRPN_DEBUG_LIBRARY NAMES libvrpn.a vrpn.lib vrpn
          HINTS $ENV{G}/src/vrpn/7.30/vrpn/build/${GBUILDSTR}/Debug ${VRPN_LIB_DIR})

set(LIBVRPN_INCLUDE_DIRS ${LIBVRPN_INCLUDE_DIR} )

#if both libs were found, set them both
if(LIBVRPN_OPT_LIBRARY AND LIBVRPN_DEBUG_LIBRARY)
  set(LIBVRPN_OPT_LIBRARIES optimized ${LIBVRPN_OPT_LIBRARY} )
  set(LIBVRPN_DEBUG_LIBRARIES debug ${LIBVRPN_DEBUG_LIBRARY} )
#if only opt is found, use it for both
elseif(LIBVRPN_OPT_LIBRARY AND NOT LIBVRPN_DEBUG_LIBRARY)
  set(LIBVRPN_OPT_LIBRARIES optimized ${LIBVRPN_OPT_LIBRARY} )
  set(LIBVRPN_DEBUG_LIBRARIES debug ${LIBVRPN_OPT_LIBRARY} )
#if only debug is found, use it for both
elseif(LIBVRPN_DEBUG_LIBRARY AND NOT LIBVRPN_OPT_LIBRARY)
  set(LIBVRPN_OPT_LIBRARIES optimized ${LIBVRPN_DEBUG_LIBRARY} )
  set(LIBVRPN_DEBUG_LIBRARIES debug ${LIBVRPN_DEBUG_LIBRARY} )
endif()

set(LIBVRPN_ALL_LIBRARIES "")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBVRPN  DEFAULT_MSG
                                  LIBVRPN_OPT_LIBRARIES LIBVRPN_DEBUG_LIBRARIES LIBVRPN_INCLUDE_DIR)

if(NOT LIBVRPN_FOUND)
  set(VRPN_INCLUDE_DIR $ENV{VRPN_INCLUDE_DIR} CACHE PATH "Set the directory location of the VRPN include folder")
  set(VRPN_LIB_DIR $ENV{VRPN_LIB_DIR} CACHE PATH "Set the directory location of the VRPN lib folder")
endif()

mark_as_advanced(LIBVRPN_INCLUDE_DIR LIBVRPN_OPT_LIBRARY LIBVRPN_DEBUG_LIBRARY )
