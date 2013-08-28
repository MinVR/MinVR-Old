/**
 @file BinaryFormat.h
 @maintainer Morgan McGuire, http://graphics.cs.williams.edu
 
 @author  2005-06-03
 @edited  2005-06-03

 Copyright 2000-2005, Morgan McGuire.
 All rights reserved.
 */

#ifndef G3DLITE_BINARYFORMAT_H
#define G3DLITE_BINARYFORMAT_H

#include "G3DLite/platform.h"
#include "G3DLite/g3dmath.h"

namespace G3DLite {

class Vector2;
class Vector2int16;
class Vector3;
class Vector3int16;
class Vector4;
class Vector4int16;
class Color3;
class Color3uint8;
class Color4;
class Color4uint8;

/**
 Some values like float16 and int128 have no current CPU data structure that implements them but are useful
 for file formats and for GPUs.

 CHUNK_BINFMT data follows the protocol.
 */
// Must be packed int 16 bits for the chunk reader
// We can't name these just "INT8" etc. because some libraries #define names like that
enum BinaryFormat {
    FIRST_BINFMT = 1000,

    BOOL8_BINFMT,
    UINT8_BINFMT, INT8_BINFMT, UINT16_BINFMT, INT16_BINFMT, UINT32_BINFMT, INT32_BINFMT, UINT64_BINFMT, INT64_BINFMT, UINT128_BINFMT, INT128_BINFMT,
    FLOAT16_BINFMT, FLOAT32_BINFMT, FLOAT64_BINFMT,
    VECTOR2_BINFMT, VECTOR2INT16_BINFMT,
    VECTOR3_BINFMT, VECTOR3INT16_BINFMT,
    VECTOR4_BINFMT, VECTOR4INT16_BINFMT,
    COLOR3_BINFMT, COLOR3UINT8_BINFMT, COLOR3INT16_BINFMT,
    COLOR4_BINFMT, COLOR4UINT8_BINFMT, COLOR4INT16_BINFMT,
    STRING_BINFMT, STRINGEVEN_BINFMT, STRING8_BINFMT, STRING16_BINFMT, STRING32_BINFMT,

    CHUNK_BINFMT,

    CUSTOM_BINFMT,

    LAST_BINFMT
};

}

/** A macro that maps G3D types to format constants.
    (e.g. binaryFormatOf(Vector3) == VECTOR3_BINFMT).
*/
// This implementation is designed to meet the following constraints:
//   1. Work around the many MSVC++ partial template bugs
//   2. Work for primitive types (e.g. int)
#define binaryFormatOfG3DLITE(T) (G3DLite::_internal::_BinaryFormat<T>::x())

namespace G3DLite {
namespace _internal {


template<class T> class _BinaryFormat {
public:
    static BinaryFormat x() {
        return CUSTOM_BINFMT;
    }
};
}}


/**
 Macro to declare the underlying format (as will be returned by glFormatOf)
 of a type.  For example,

  <PRE>
    DECLARE_BINARYFORMATOF(Vector4, VECTOR4_BINFMT)
  </PRE>

  Use this so you can make vertex arrays of your own classes and not just 
  the standard ones.
 */
#define DECLARE_BINARYFORMATOFG3DLITE(CType, EnumType)      \
namespace G3DLite {                                      \
    namespace _internal {                            \
        template<> class _BinaryFormat<CType> {      \
        public:                                      \
        static BinaryFormat x()  {                   \
                return EnumType;                     \
            }                                        \
        };                                           \
    }                                                \
}

DECLARE_BINARYFORMATOFG3DLITE( bool,          BOOL8_BINFMT )

DECLARE_BINARYFORMATOFG3DLITE( uint8,         UINT8_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( int8,          INT8_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( uint16,        UINT16_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( int16,         INT16_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( uint32,        UINT32_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( int32,         INT32_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( uint64,        UINT64_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( int64,         INT64_BINFMT )

DECLARE_BINARYFORMATOFG3DLITE( float32,       FLOAT32_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( float64,       FLOAT64_BINFMT )

DECLARE_BINARYFORMATOFG3DLITE( Vector2,       VECTOR2_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Vector2int16,  VECTOR2INT16_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Vector3,       VECTOR3_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Vector3int16,  VECTOR3INT16_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Vector4,       VECTOR4_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Vector4int16,  VECTOR4INT16_BINFMT )

DECLARE_BINARYFORMATOFG3DLITE( Color3,        COLOR3_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Color3uint8,   COLOR3UINT8_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Color4,        COLOR4_BINFMT )
DECLARE_BINARYFORMATOFG3DLITE( Color4uint8,   COLOR4UINT8_BINFMT )

namespace G3DLite {

/** Returns -1 if the format is custom, otherwise the byte size
    of a single element in this format.*/ 
int32 byteSize(BinaryFormat f);


} //G3D

#endif 
