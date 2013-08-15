/**
 @file G3D.h

 This header includes all of the G3D libraries in
 appropriate namespaces.

 @maintainer Morgan McGuire, http://graphics.cs.williams.edu

 @created 2001-08-25
 @edited  2010-03-20

 Copyright 2000-2010, Morgan McGuire.
 All rights reserved.
*/

#ifndef G3DLITE_h
#define G3DLITE_h

#include "G3DLite/AABox.h"
#include "G3DLite/Any.h"
#include "G3DLite/Array.h"
#include "G3DLite/AtomicInt32.h"
#include "G3DLite/BinaryFormat.h"
#include "G3DLite/BinaryInput.h"
#include "G3DLite/BinaryOutput.h"
#include "G3DLite/BoundsTrait.h"
#include "G3DLite/Box.h"
#include "G3DLite/Capsule.h"
#include "G3DLite/CollisionDetection.h"
#include "G3DLite/Color1.h"
#include "G3DLite/Color1uint8.h"
#include "G3DLite/Color3.h"
#include "G3DLite/Color3uint8.h"
#include "G3DLite/Color4.h"
#include "G3DLite/Color4uint8.h"
#include "G3DLite/CoordinateFrame.h"
#include "G3DLite/Crypto.h"
#include "G3DLite/Cylinder.h"
#include "G3DLite/debugPrintf.h"
#include "G3DLite/EqualsTrait.h"
#include "G3DLite/FileSystem.h"
#include "G3DLite/fileutils.h"
#include "G3DLite/format.h"
#include "G3DLite/g3dfnmatch.h"
#include "G3DLite/G3DGameUnits.h"
#include "G3DLite/g3dmath.h"
#include "G3DLite/GMutex.h"
#include "G3DLite/GThread.h"
#include "G3DLite/HashTrait.h"
#include "G3DLite/Line.h"
#include "G3DLite/LineSegment.h"
#include "G3DLite/Log.h"
#include "G3DLite/Matrix3.h"
#include "G3DLite/Matrix4.h"
#include "G3DLite/MemoryManager.h"
#include "G3DLite/ParseError.h"
#include "G3DLite/PhysicsFrame.h"
#include "G3DLite/Plane.h"
#include "G3DLite/platform.h"
#include "G3DLite/PositionTrait.h"
#include "G3DLite/Quat.h"
#include "G3DLite/Random.h"
#include "G3DLite/Ray.h"
#include "G3DLite/Rect2D.h"
#include "G3DLite/ReferenceCount.h"
#include "G3DLite/RegistryUtil.h"
#include "G3DLite/Set.h"
#include "G3DLite/Sphere.h"
#include "G3DLite/Spline.h"
#include "G3DLite/stringutils.h"
#include "G3DLite/System.h"
#include "G3DLite/Table.h"
#include "G3DLite/TextInput.h"
#include "G3DLite/TextOutput.h"
#include "G3DLite/Triangle.h"
#include "G3DLite/uint128.h"
#include "G3DLite/units.h"
#include "G3DLite/UprightFrame.h"
#include "G3DLite/Vector2.h"
#include "G3DLite/Vector2int16.h"
#include "G3DLite/Vector3.h"
#include "G3DLite/Vector3int16.h"
#include "G3DLite/Vector3int32.h"
#include "G3DLite/Vector4.h"
#include "G3DLite/Vector4int8.h"
#include "G3DLite/vectorMath.h"

#endif