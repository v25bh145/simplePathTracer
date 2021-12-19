//
// Created by v25bh on 2021/11/23.
//

#include "geometry.h"

namespace pathTracer {
    unsigned Geometry::RTCInnerObjNumber = 0;

    Geometry::Geometry() {}

    Vector3f getMax(Vector3f p1, Vector3f p2) {
        return {
                p1.x() < p2.x() ? p2.x() : p1.x(),
                p1.y() < p2.y() ? p2.y() : p1.y(),
                p1.z() < p2.z() ? p2.z() : p1.z()
        };
    }
    Vector3f getMin(Vector3f p1, Vector3f p2) {
        return {
                p1.x() < p2.x() ? p1.x() : p2.x(),
                p1.y() < p2.y() ? p1.y() : p2.y(),
                p1.z() < p2.z() ? p1.z() : p2.z()
        };
    }
}