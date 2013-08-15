/**
 @file UprightFrame.h

 @author Morgan McGuire, http://graphics.cs.williams.edu
 */

#ifndef G3D_UPRIGHTFRAME_H
#define G3D_UPRIGHTFRAME_H

#include "G3DLite/platform.h"
#include "G3DLite/Spline.h"
#include "G3DLite/Vector3.h"
#include "G3DLite/CoordinateFrame.h"

namespace G3DLite {

/**
 Coordinate frame expressed in Euler angles.
 Unlike a G3DLite::Quat, UprightFrame always keeps the reference frame from rolling about its own z axis.
 Particularly useful for cameras.

 @sa G3DLite::CoordinateFrame, G3DLite::Matrix4, G3DLite::PhysicsFrame, G3DLite::UprightSpline, G3DLite::UprightSplineManipulator
 */
class UprightFrame {
public:

    Vector3         translation;

    /** -pi/2 < pitch < pi/2 in radians about the X-axis */
    float           pitch;

    /** In radians about the Y-axis */
    float           yaw;

    inline UprightFrame(const Vector3& t = Vector3::zero(), float p = 0, float y = 0) 
        : translation(t), pitch(p), yaw(y) {}

    UprightFrame(const CoordinateFrame& cframe);
    
    CoordinateFrame toCoordinateFrame() const;

    /** Supports implicit cast to CoordinateFrame */
    inline operator CoordinateFrame() const {
        return toCoordinateFrame();
    }

    /** Required for use with spline */
    UprightFrame operator+(const UprightFrame& other) const;

    /** Required for use with spline */
    UprightFrame operator*(const float k) const;

    /**
       Unwraps the yaw values in the elements of the array such that
       they still represent the same angles but strictly increase/decrease
       without wrapping about zero.  For use with Spline<UprightFrame>
     */
    static void unwrapYaw(UprightFrame* a, int N);

    void serialize(class BinaryOutput& b) const;
    void deserialize(class BinaryInput& b);
};

/** Shortest-path linear velocity spline for camera positions.  Always keeps the camera from rolling.
@sa G3DLite::UprightSplineManipulator, G3DLite::UprightFrame
*/
class UprightSpline : public Spline<UprightFrame> {
protected:

    virtual void ensureShortestPath(UprightFrame* A, int N) const {
        UprightFrame::unwrapYaw(A, N);
    }

public:
    
    void serialize(class BinaryOutput& b) const;
    void deserialize(class BinaryInput& b);

};

}

#endif
