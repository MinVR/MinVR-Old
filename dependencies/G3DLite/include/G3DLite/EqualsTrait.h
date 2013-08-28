/**
  @file EqualsTrait.h

  @maintainer Morgan McGuire, http://graphics.cs.williams.edu
  @created 2008-10-01
  @edited  2008-10-01
  Copyright 2000-2009, Morgan McGuire.
  All rights reserved.
 */

#ifndef G3DLITE_EQUALSTRAIT_H
#define G3DLITE_EQUALSTRAIT_H

#include "G3DLite/platform.h"

namespace G3DLite {

/** Default implementation of EqualsTrait.
    @see G3DLite::Table for specialization requirements.
*/
template<typename Key> struct EqualsTrait {
    static bool equals(const Key& a, const Key& b) {
        return a == b;
    }
};

} // namespace G3DLite

#endif

