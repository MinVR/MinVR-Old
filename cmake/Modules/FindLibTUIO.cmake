find_path(LIBTUIO_INCLUDE_DIR TuioClient.h
          HINTS $ENV{G}/src/TUIO/client-lib/ $ENV{TUIO_INCLUDE_DIR})

find_library(LIBTUIO_OPT_LIBRARY NAMES libTUIO.a TUIO.lib TUIO
          HINTS $ENV{G}/src/TUIO/client-lib/build/${GBUILDSTR}/Release $ENV{TUIO_LIB_DIR})

find_library(LIBTUIO_DEBUG_LIBRARY NAMES libTUIO.a TUIO.lib TUIO
          HINTS $ENV{G}/src/TUIO/client-lib/build/${GBUILDSTR}/Debug $ENV{TUIO_LIB_DIR})

set(LIBTUIO_ALL_LIBRARIES "")
set(LIBTUIO_INCLUDE_DIRS ${LIBTUIO_INCLUDE_DIR} )

if(LIBTUIO_OPT_LIBRARY AND LIBTUIO_DEBUG_LIBRARY)
  set(LIBTUIO_OPT_LIBRARIES optimized ${LIBTUIO_OPT_LIBRARY} )
  set(LIBTUIO_DEBUG_LIBRARIES debug ${LIBTUIO_DEBUG_LIBRARY} )
#if only opt is found, use it for both
elseif(LIBTUIO_OPT_LIBRARY AND NOT LIBTUIO_DEBUG_LIBRARY)
  set(LIBTUIO_OPT_LIBRARIES optimized ${LIBTUIO_OPT_LIBRARY} )
  set(LIBTUIO_DEBUG_LIBRARIES debug ${LIBTUIO_OPT_LIBRARY} )
#if only debug is found, use it for both
elseif(LIBTUIO_DEBUG_LIBRARY AND NOT LIBTUIO_OPT_LIBRARY)
  set(LIBTUIO_OPT_LIBRARIES optimized ${LIBTUIO_DEBUG_LIBRARY} )
  set(LIBTUIO_DEBUG_LIBRARIES debug ${LIBTUIO_DEBUG_LIBRARY} )
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBTUIO  DEFAULT_MSG
                                  LIBTUIO_OPT_LIBRARY LIBTUIO_DEBUG_LIBRARY LIBTUIO_INCLUDE_DIR)

mark_as_advanced(LIBTUIO_INCLUDE_DIR LIBTUIO_OPT_LIBRARY LIBTUIO_DEBUG_LIBRARY )