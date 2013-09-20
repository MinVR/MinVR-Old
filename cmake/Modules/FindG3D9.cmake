find_path(LIBG3D_INCLUDE_DIR G3D/G3D.h
          HINTS $ENV{G}/src/G3D/G3D-9.0/include $ENV{G3D9_INCLUDE_DIR} ${G3D9_INCLUDE_DIR})

if (CMAKE_SIZEOF_VOID_P EQUAL 8 )
find_library(GLG3D NAMES libGLG3D_x64.a GLG3D_x64.lib GLG3D_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(G3D NAMES libG3D_x64.a G3D_x64.lib G3D_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(GLG3Dd NAMES libGLG3D_x64d.a GLG3D_x64d.lib GLG3D_x64d
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(G3Dd NAMES libG3D_x64d.a G3D_x64d.lib G3D_x64d
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(zlib NAMES libz_x64.a z_x64.lib zlib_x64.lib z zlib_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(zip NAMES libzip_x64.a zip_x64.lib zip_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(freeimage NAMES libfreeimage_x64.a freeimage_x64.lib freeimage_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(freeimaged NAMES libfreeimage_x64d.a freeimage_x64d.lib freeimage_x64d
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(assimpd NAMES assimp_x64d.a assimp_x64d.lib assimp_x64d
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(assimp NAMES assimp_x64.a assimp_x64.lib assimp_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(avcodec NAMES avcodec_x64.a avcodec_x64.lib avcodec_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(avformat NAMES avformat_x64.a avformat_x64.lib avformat_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(avutil NAMES avutil_x64.a avutil_x64.lib avutil_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(enet NAMES enet_x64.a enet_x64.lib enet_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(glfw NAMES glfw_x64.a glfw_x64.lib glfw_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(swscale NAMES swscale_x64.a swscale_x64.lib swscale_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(civetweb NAMES civetweb_x64.a civetweb_x64.lib civetweb_x64
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(civetwebd NAMES civetweb_x64d.a civetweb_x64d.lib civetweb_x64d
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
else()
find_library(GLG3D NAMES libGLG3D.a GLG3D.lib GLG3D
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(G3D NAMES libG3D.a G3D.lib G3D
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(GLG3Dd NAMES libGLG3Dd.a GLG3Dd.lib GLG3Dd
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(G3Dd NAMES libG3Dd.a G3Dd.lib G3Dd
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(zlib NAMES libz.a z.lib zlib.lib z zlib
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(zip NAMES libzip.a zip.lib zip
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(freeimage NAMES libfreeimage.a freeimage.lib freeimage
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(freeimaged NAMES libfreeimaged.a freeimaged.lib freeimaged
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(assimpd NAMES assimpd.a assimpd.lib assimpd
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(assimp NAMES assimp.a assimp.lib assimp
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(avcodec NAMES avcodec.a avcodec.lib avcodec
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(avformat NAMES avformat.a avformat.lib avformat
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(avutil NAMES avutil.a avutil.lib avutil
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(enet NAMES enet.a enet.lib enet
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(glfw NAMES glfw.a glfw.lib glfw
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(swscale NAMES swscale.a swscale.lib swscale
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})

find_library(civetweb NAMES civetweb.a civetweb.lib civetweb
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})
			 
find_library(civetwebd NAMES civetwebd.a civetwebd.lib civetwebd
             HINTS $ENV{G}/src/G3D/G3D-9.0/${GBUILDSTR} $ENV{G3D9_LIB_DIR} ${G3D9_LIB_DIR})			 
endif(CMAKE_SIZEOF_VOID_P EQUAL 8 )


set(LIBG3D_DEBUG_LIBRARIES debug ${GLG3Dd} debug ${G3Dd} debug ${freeimaged} debug ${assimpd} debug ${civetwebd})
set(LIBG3D_OPT_LIBRARIES optimized ${GLG3D} optimized ${G3D} optimized ${freeimage} optimized ${assimp} optimized ${civetweb})
set(LIBG3D_ALL_LIBRARIES ${zlib} ${zip} ${avcodec} ${avformat} ${avutil} ${enet} ${glfw} ${swscale})
set(G3D9_LIBRARIES ${LIBG3D_DEBUG_LIBRARIES} ${LIBG3D_OPT_LIBRARIES} ${LIBG3D_ALL_LIBRARIES})
set(G3D9_INCLUDE_DIRS ${LIBG3D_INCLUDE_DIR} )


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBNAME_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(G3D9  DEFAULT_MSG
                                  GLG3D G3D GLG3Dd G3Dd zlib zip freeimaged freeimage assimpd assimp civetwebd civetweb avcodec avformat avutil enet glfw LIBG3D_INCLUDE_DIR)

if(NOT G3D9_FOUND)
  set(G3D9_INCLUDE_DIR $ENV{G3D9_INCLUDE_DIR} CACHE PATH "Set the directory location of the G3D include folder")
  set(G3D9_LIB_DIR $ENV{G3D9_LIB_DIR} CACHE PATH "Set the directory location of the G3D libs folder")
endif()
