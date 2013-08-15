/**
 @file Rect2D.cpp
 
 @maintainer Morgan McGuire, http://graphics.cs.williams.edu
  
  @created 2003-11-13
  @created 2009-11-16

  Copyright 2000-2009, Morgan McGuire.
  All rights reserved.
 */

#include "G3DLite/platform.h"
#include "G3DLite/Rect2D.h"
#include "G3DLite/Any.h"
#include "G3DLite/stringutils.h"

namespace G3DLite {

/** \param any Must either Rect2D::xywh(#, #, #, #) or Rect2D::xyxy(#, #, #, #)*/
Rect2D::Rect2D(const Any& any) {
    any.verifyName("Rect2D");
    any.verifyType(Any::ARRAY);
    any.verifySize(4);
    if (toUpper(any.name()) == "RECT2D::XYWH") {
        *this = Rect2D::xywh(any[0], any[1], any[2], any[3]);
    } else {
        any.verifyName("Rect2D::xyxy");
        *this = Rect2D::xyxy(any[0], any[1], any[2], any[3]);
    }
}


/** Converts the Rect2D to an Any. */
Rect2D::operator Any() const {
    Any any(Any::ARRAY, "Rect2D::xywh");
    any.append(x0(), y0(), width(), height());
    return any;
}

}
