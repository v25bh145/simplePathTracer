//
// Created by v25bh on 2021/12/7.
//

#ifndef SIMPLEPATHTRACER_MATHHELPER_H
#define SIMPLEPATHTRACER_MATHHELPER_H

#include "default.h"


namespace pathTracer {
    void orthogonal(Vector3f n, Vector3f &s, Vector3f &t);
    string vector3fToString(Vector3f v);
    string vector2fToString(Vector2f v);
    Vector3f sameSide(Vector3f n, Vector3f w);
    bool vector3fEqualTo0(Vector3f vector3f);
    float triangleArea(Vector3f p1, Vector3f p2, Vector3f p3);
    int clampi(int v, int a, int b);
    float clampf(float v, float a, float b);
}


#endif //SIMPLEPATHTRACER_MATHHELPER_H
