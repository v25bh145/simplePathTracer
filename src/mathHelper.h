//
// Created by v25bh on 2021/12/7.
//

#ifndef SIMPLEPATHTRACER_MATHHELPER_H
#define SIMPLEPATHTRACER_MATHHELPER_H

#include "default.h"


namespace pathTracer {
    void orthogonal(Vector3f n, Vector3f &s, Vector3f &t);
    string vector3fToString(Vector3f v);
    Vector3f sameSide(Vector3f n, Vector3f w);
    bool vector3fEqualTo0(Vector3f vector3f);
}


#endif //SIMPLEPATHTRACER_MATHHELPER_H
