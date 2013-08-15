cmake_minimum_required (VERSION 2.8.2)
set (CMAKE_VERBOSE_MAKEFILE TRUE)


project (G3DLite)


set (SOURCEFILES 
source/Any.cpp
source/AABox.cpp
source/BinaryFormat.cpp
source/BinaryInput.cpp
source/BinaryOutput.cpp
source/Box.cpp
source/Capsule.cpp
source/CollisionDetection.cpp
source/Color3.cpp
source/Color3uint8.cpp
source/Color4.cpp
source/Color4uint8.cpp
source/CoordinateFrame.cpp
source/Crypto.cpp
source/Cylinder.cpp
source/debugAssert.cpp
source/FileSystem.cpp
source/GThread.cpp
source/Line.cpp
source/LineSegment.cpp
source/Log.cpp
source/Matrix3.cpp
source/Matrix4.cpp
source/MemoryManager.cpp
source/PhysicsFrame.cpp
source/Plane.cpp
source/Quat.cpp
source/Random.cpp
source/Ray.cpp
source/Rect2D.cpp
source/ReferenceCount.cpp
source/RegistryUtil.cpp
source/Sphere.cpp
source/System.cpp
source/TextInput.cpp
source/TextOutput.cpp
source/Triangle.cpp
source/UprightFrame.cpp
source/Vector2.cpp
source/Vector2int16.cpp
source/Vector3.cpp
source/Vector3int16.cpp
source/Vector3int32.cpp
source/Vector4.cpp
source/Vector4int8.cpp
source/fileutils.cpp
source/format.cpp
source/g3dfnmatch.cpp
source/g3dmath.cpp
source/stringutils.cpp
source/uint128.cpp
)


set (HEADERFILES
include/G3DLite/G3DLite.h 
include/G3DLite/AABox.h
include/G3DLite/Any.h
include/G3DLite/Array.h
include/G3DLite/AtomicInt32.h
include/G3DLite/BinaryFormat.h
include/G3DLite/BinaryInput.h
include/G3DLite/BinaryOutput.h
include/G3DLite/BoundsTrait.h
include/G3DLite/Box.h
include/G3DLite/Capsule.h
include/G3DLite/CollisionDetection.h
include/G3DLite/Color1.h
include/G3DLite/Color1uint8.h
include/G3DLite/Color3.h
include/G3DLite/Color3uint8.h
include/G3DLite/Color4.h
include/G3DLite/Color4uint8.h
include/G3DLite/CoordinateFrame.h
include/G3DLite/Crypto.h
include/G3DLite/Cylinder.h
include/G3DLite/EqualsTrait.h
include/G3DLite/FileSystem.h
include/G3DLite/G3DGameUnits.h
include/G3DLite/GMutex.h
include/G3DLite/GThread.h
include/G3DLite/HashTrait.h
include/G3DLite/Line.h
include/G3DLite/LineSegment.h
include/G3DLite/Log.h
include/G3DLite/Matrix3.h
include/G3DLite/Matrix4.h
include/G3DLite/MemoryManager.h
include/G3DLite/ParseError.h
include/G3DLite/PhysicsFrame.h
include/G3DLite/Plane.h
include/G3DLite/PositionTrait.h
include/G3DLite/Quat.h
include/G3DLite/Random.h
include/G3DLite/Ray.h
include/G3DLite/Rect2D.h
include/G3DLite/ReferenceCount.h
include/G3DLite/RegistryUtil.h
include/G3DLite/Set.h
include/G3DLite/Sphere.h
include/G3DLite/Spline.h
include/G3DLite/System.h
include/G3DLite/Table.h
include/G3DLite/TextInput.h
include/G3DLite/TextOutput.h
include/G3DLite/Triangle.h
include/G3DLite/UprightFrame.h
include/G3DLite/Vector2.h
include/G3DLite/Vector2int16.h
include/G3DLite/Vector3.h
include/G3DLite/Vector3int16.h
include/G3DLite/Vector3int32.h
include/G3DLite/Vector4.h
include/G3DLite/Vector4int8.h
include/G3DLite/vectorMath.h
include/G3DLite/debug.h
include/G3DLite/debugAssert.h
include/G3DLite/debugPrintf.h
include/G3DLite/fileutils.h
include/G3DLite/format.h
include/G3DLite/g3dfnmatch.h
include/G3DLite/g3dmath.h
include/G3DLite/platform.h
include/G3DLite/stringutils.h
include/G3DLite/uint128.h
include/G3DLite/units.h
)
source_group("Header Files" FILES ${HEADERFILES})


# Include Directories

include_directories (
  .
  ${PROJECT_SOURCE_DIR}/include
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
add_dependencies( ${PROJECT_NAME} boost )
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Dependencies")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION "${MINVR_INSTALL_DIR}/include")

