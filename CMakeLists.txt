cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

project (MinVR)

set(MINVR_VERSION_MAJOR "0")
set(MINVR_VERSION_MINOR "9")
set(MINVR_VERSION_PATCH "0")
set(MINVR_VERSION_EXTRA "")
set(MINVR_VERSION "${MINVR_VERSION_MAJOR}.${MINVR_VERSION_MINOR}")
set(MINVR_VERSION_FULL "${MINVR_VERSION}.${MINVR_VERSION_PATCH}${MINVR_VERSION_EXTRA}")

option(USE_VRPN "Enable events from vrpn devices" ON)
option(USE_TUIO "Enable generation of events from a tuio device" OFF)
option(USE_SPACENAV "Enable generation of events from the 3dconnexion SpaceNavigator device" OFF)

option(USE_APPKIT_GLFW "Enable to use the GLFW app kit" ON)
option(USE_APPKIT_G3D9 "Enable to use the G3D9 app kit" OFF)
option(USE_APPKIT_GLUT "Enable to use the GLut app kit" OFF)

option(BUILD_USE_SOLUTION_FOLDERS "Enable grouping of projects in Visual Studio" ON)
option(BUILD_EXAMPLES "Enable to build app kit example projects" ON)
option(BUILD_DEPENDENCIES "If enabled, dependencies will be downloaded and built if an installed version is not found" ON)
option(BUILD_DOCUMENTATION "If enable, cmake attempts to find Doxygen and build the API documentation" OFF)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ${BUILD_USE_SOLUTION_FOLDERS})

set(MINVR_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/MinVR" CACHE PATH "Install location for MinVR")
add_definitions(-DINSTALLPATH="${MINVR_INSTALL_DIR}")

if (BUILD_DOCUMENTATION)
	set(DOXYGEN_SKIP_DOT TRUE)
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		add_subdirectory(docs)
	endif()
endif()

if (USE_VRPN)
	find_package(LibVRPN)
	if (LIBVRPN_FOUND)
		add_definitions(-DUSE_VRPN)
		set(LIBS_OPT ${LIBS_OPT} ${LIBVRPN_OPT_LIBRARIES})
		set(LIBS_DEBUG ${LIBS_DEBUG} ${LIBVRPN_DEBUG_LIBRARIES})
		set(LIBS_ALL ${LIBS_ALL} ${LIBVRPN_ALL_LIBRARIES})
		include_directories(${LIBVRPN_INCLUDE_DIRS})
	elseif(BUILD_DEPENDENCIES)
		set(vrpn_checkout_Dir ${CMAKE_SOURCE_DIR}/dependencies/vrpn)
		make_directory(${vrpn_checkout_Dir})
        message("   Library will be built from source in ${vrpn_checkout_Dir}.")
		include(ExternalProject)
		set(vrpn_PREFIX "${vrpn_checkout_Dir}")
		set(vrpn_INSTALL_DIR "${MINVR_INSTALL_DIR}/vrpn")
		set(vrpn_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${vrpn_INSTALL_DIR} -DVRPN_BUILD_DIRECTSHOW_VIDEO_SERVER=OFF -DVRPN_BUILD_HID_GUI=OFF -DVRPN_BUILD_JAVA=OFF -DVRPN_BUILD_PYTHON=OFF -DVRPN_USE_DIRECTINPUT=OFF -DVRPN_USE_DIRECTSHOW=OFF)
		set(vrpn_DIR "${vrpn_INSTALL_DIR}")


		if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
			set(vrpn_CMAKE_ARGS ${vrpn_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD=c++11 -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY=libc++)
		elseif(MSVC)
				set(vrpn_CMAKE_ARGS ${vrpn_CMAKE_ARGS} -DVRPN_USE_HID=ON -DVRPN_HID_DEBUGGING=ON -DVRPN_USE_LOCAL_HIDAPI=ON)
		endif()

		ExternalProject_add(vrpn
				PREFIX ${vrpn_PREFIX}
				URL http://www.cs.unc.edu/Research/vrpn/downloads/vrpn_07_30.zip
				INSTALL_DIR ${vrpn_INSTALL_DIR}
				CMAKE_ARGS ${vrpn_CMAKE_ARGS}
		 )		 
		set_property(TARGET vrpn PROPERTY FOLDER "Dependencies")
		set(VRPN_INCLUDE_DIR ${vrpn_INSTALL_DIR}/include CACHE INTERNAL "Directory of VRPN header files")
		if(MSVC)
			set(VRPN_LIBRARY ${vrpn_INSTALL_DIR}/lib/vrpn.lib CACHE INTERNAL "VRPN lib file")
		else()
			set(VRPN_LIBRARY ${vrpn_INSTALL_DIR}/lib/libvrpn.a CACHE INTERNAL "VRPN lib file")
		endif()	
		add_definitions(-DUSE_VRPN)
		set(LIBS_ALL ${LIBS_ALL} ${VRPN_LIBRARY})
		include_directories(${VRPN_INCLUDE_DIR})
	endif()
endif()

if (USE_TUIO)
	find_package(LibTUIO)
	if (LIBTUIO_FOUND)
		add_definitions(-DUSE_TUIO)
		set(LIBS_OPT ${LIBS_OPT} ${LIBTUIO_OPT_LIBRARIES})
		set(LIBS_DEBUG ${LIBS_DEBUG} ${LIBTUIO_DEBUG_LIBRARIES})
		set(LIBS_ALL ${LIBS_ALL} ${LIBTUIO_ALL_LIBRARIES})
		include_directories(${LIBTUIO_INCLUDE_DIRS})
	elseif(BUILD_DEPENDENCIES)
		set(tuio_checkout_Dir ${CMAKE_SOURCE_DIR}/dependencies/tuio)
		make_directory(${tuio_checkout_Dir})
        message("   Library will be built from source in ${tuio_checkout_Dir}.")
		include(ExternalProject)
		set(tuio_PREFIX "${tuio_checkout_Dir}")
		set(tuio_INSTALL_DIR "${MINVR_INSTALL_DIR}/tuio")
		set(tuio_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${tuio_INSTALL_DIR}")
		set(tuio_DIR "${tuio_INSTALL_DIR}")
		set(tuio_PATCH "${CMAKE_COMMAND} -E copy ${tuio_checkout_Dir}/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt")

		if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
			set(tuio_CMAKE_ARGS ${tuio_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD=c++11 -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY=libc++)
		endif()

		ExternalProject_add(TUIO
				PREFIX ${tuio_PREFIX}
				URL http://prdownloads.sourceforge.net/reactivision/TUIO_CPP-1.4.zip
				INSTALL_DIR ${tuio_INSTALL_DIR}
				CMAKE_ARGS ${tuio_CMAKE_ARGS}
				PATCH_COMMAND ${tuio_PATCH}
		 )		 
		set_property(TARGET TUIO PROPERTY FOLDER "Dependencies")
		set(TUIO_INCLUDE_DIR ${tuio_INSTALL_DIR}/include CACHE INTERNAL "Directory of TUIO header files")
		if(MSVC)
			set(TUIO_LIBRARY ${tuio_INSTALL_DIR}/lib/TUIO.lib CACHE INTERNAL "TUIO lib file")
		else()
			set(TUIO_LIBRARY ${tuio_INSTALL_DIR}/lib/libTUIO.a CACHE INTERNAL "TUIO lib file")
		endif()
		add_definitions(-DUSE_TUIO)
		set(LIBS_ALL ${LIBS_ALL} ${TUIO_LIBRARY})
		include_directories(${TUIO_INCLUDE_DIR})
	endif()
endif()

if (USE_SPACENAV)
	find_package(LibSPACENAV REQUIRED)
	add_definitions(-DUSE_SPACENAV)
	set(LIBS_OPT ${LIBS_OPT} ${LIBSPACENAV_OPT_LIBRARIES})
	set(LIBS_DEBUG ${LIBS_DEBUG} ${LIBSPACENAV_DEBUG_LIBRARIES})
	set(LIBS_ALL ${LIBS_ALL} ${LIBSPACENAV_ALL_LIBRARIES})
	include_directories(${LIBSPACENAV_INCLUDE_DIRS})
endif()

find_package(OpenGL REQUIRED)
set(LIBS_ALL ${LIBS_ALL} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
include_directories(${OPENGL_INCLUDE_DIR})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_ADDITIONAL_VERSIONS "1.54" "1.54.0")
find_package(Boost 1.54.0 COMPONENTS chrono date_time system thread log log_setup filesystem)
if(Boost_FOUND)
    message("Using installed version of Boost")
elseif(BUILD_DEPENDENCIES)
     set(Boost_Download_Dir ${CMAKE_SOURCE_DIR}/dependencies/boost)
	 make_directory(${Boost_Download_Dir})
     message("   Library will be built from source in ${Boost_Download_Dir}.")
     
     include(ExternalProject)
     set(boost_PREFIX "${Boost_Download_Dir}")
     set(boost_INSTALL_DIR "${MINVR_INSTALL_DIR}/boost")
     if (MSVC)
        set(boost_CONFIGURE_COMMAND <SOURCE_DIR>/bootstrap.bat)
        set(boost_BUILD_COMMAND <SOURCE_DIR>/b2 install --prefix=${boost_INSTALL_DIR} --with-thread --with-chrono --with-date_time --with-log --with-filesystem toolset=msvc-10.0 link=static runtime-link=shared threading=multi address-model=64 variant=debug,release)
     else()
        set(boost_CONFIGURE_COMMAND <SOURCE_DIR>/bootstrap.sh cxxflags="-std=c++11 -stdlib=libc++" --with-toolset=clang)
        set(boost_BUILD_COMMAND <SOURCE_DIR>/b2 install --prefix=${boost_INSTALL_DIR} --with-thread --with-chrono --with-date_time --with-log --with-filesystem toolset=clang cxxflags=-std=c++11 cxxflags=-stdlib=libc++ linkflags=-stdlib=libc++ link=static runtime-link=shared threading=multi address-model=64 variant=release)
     endif()
     ExternalProject_add(boost
     		  PREFIX ${boost_PREFIX}
			  URL http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.bz2
     		  URL_MD5 15cb8c0803064faef0c4ddf5bc5ca279
     		  CONFIGURE_COMMAND ${boost_CONFIGURE_COMMAND}
              BUILD_COMMAND ${boost_BUILD_COMMAND}
     		  INSTALL_DIR ${boost_INSTALL_DIR}
			  INSTALL_COMMAND ""
     		  BUILD_IN_SOURCE 1
     )
	 
    set_property(TARGET boost PROPERTY FOLDER "Dependencies")
    
    set( Boost_DIR ${boost_INSTALL_DIR} CACHE PATH "Boost Root Directory")
    set( Boost_LIBRARY_DIRS ${boost_INSTALL_DIR}/lib CACHE PATH "Boost Library directory" FORCE)
    
    if(MSVC)
        set( Boost_INCLUDE_DIR ${boost_INSTALL_DIR}/include/boost-1_54 CACHE PATH "Boost Include directory" FORCE)
        set( Boost_LIBRARIES optimized ${Boost_LIBRARY_DIRS}/libboost_chrono-vc100-mt-1_54.lib optimized ${Boost_LIBRARY_DIRS}/libboost_date_time-vc100-mt-1_54.lib optimized ${Boost_LIBRARY_DIRS}/libboost_system-vc100-mt-1_54.lib optimized ${Boost_LIBRARY_DIRS}/libboost_thread-vc100-mt-1_54.lib optimized ${Boost_LIBRARY_DIRS}/libboost_filesystem-vc100-mt-1_54.lib optimized ${Boost_LIBRARY_DIRS}/libboost_log_setup-vc100-mt-1_54.lib optimized ${Boost_LIBRARY_DIRS}/libboost_log-vc100-mt-1_54.lib debug ${Boost_LIBRARY_DIRS}/libboost_chrono-vc100-mt-gd-1_54.lib debug ${Boost_LIBRARY_DIRS}/libboost_date_time-vc100-mt-gd-1_54.lib debug ${Boost_LIBRARY_DIRS}/libboost_system-vc100-mt-gd-1_54.lib debug ${Boost_LIBRARY_DIRS}/libboost_thread-vc100-mt-gd-1_54.lib debug ${Boost_LIBRARY_DIRS}/libboost_filesystem-vc100-mt-gd-1_54.lib debug ${Boost_LIBRARY_DIRS}/libboost_log_setup-vc100-mt-gd-1_54.lib debug ${Boost_LIBRARY_DIRS}/libboost_log-vc100-mt-gd-1_54.lib CACHE INTERNAL "Boost libraries needed to compile MINVR apps")
    else()
        set( Boost_INCLUDE_DIR ${boost_INSTALL_DIR}/include CACHE PATH "Boost Include directory" FORCE)
        set( Boost_LIBRARIES ${Boost_LIBRARY_DIRS}/libboost_chrono.a ${Boost_LIBRARY_DIRS}/libboost_date_time.a ${Boost_LIBRARY_DIRS}/libboost_system.a ${Boost_LIBRARY_DIRS}/libboost_thread.a ${Boost_LIBRARY_DIRS}/libboost_filesystem.a ${Boost_LIBRARY_DIRS}/libboost_log_setup.a ${Boost_LIBRARY_DIRS}/libboost_log.a CACHE INTERNAL "Boost libraries needed to compile MINVR apps")
    endif()
else ()
	message("   Set the BOOST_ROOT env var and re-configure, or set BUILD_BOOST to ON to download and build boost automatically.")
endif()
include_directories(${Boost_INCLUDE_DIR})

add_definitions(-DGLM_SWIZZLE)
include_directories(dependencies/glm)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # Mac OS X specific code
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(MSVC)
	add_definitions(-DNOMINMAX)
endif()

add_subdirectory (MVRCore)

#Compile the user specified App Kits
if (USE_APPKIT_GLFW)
	add_subdirectory (AppKits/AppKit_GLFW)
endif()
if (USE_APPKIT_GLUT)
	add_subdirectory (AppKits/AppKit_Glut)
endif()
if (USE_APPKIT_G3D9)
	add_subdirectory (AppKits/AppKit_G3D9)
endif()

if (BUILD_EXAMPLES)
	if(USE_APPKIT_GLFW)
		add_subdirectory(AppKits/AppKit_GLFW/example)
	endif()
	if(USE_APPKIT_G3D9)
		add_subdirectory(AppKits/AppKit_G3D9/example)
	endif()
endif()

#Configure MinVRConfig.cmake
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
include(${CMAKE_SOURCE_DIR}/cmake/minvr_minvrconfig.cmake)

install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION "${MINVR_INSTALL_DIR}/lib")
install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/ DESTINATION "${MINVR_INSTALL_DIR}/bin")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/cmake/Modules/ DESTINATION "${MINVR_INSTALL_DIR}/cmake/modules")
