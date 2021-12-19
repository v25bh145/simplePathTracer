//
// Created by v25bh on 2021/12/7.
//

#include "mathHelper.h"

namespace pathTracer {
    void orthogonal(Vector3f n, Vector3f &s, Vector3f &t) {
        if (n.x() != 0 || n.y() != 0) {
            s = { n.y(), -n.x(), 0 };
            t = { n.x() * n.z(), n.y() * n.z(), -n.x() * n.x() - n.y() * n.y() };
        }
        else if (n.x() != 0 || n.z() != 0) {
            s = { n.z(), 0, -n.x() };
            t = { -n.x() * n.y(), n.z() * n.z() + n.x() * n.x(), -n.y() * n.z() };
        }
        else {
            assert(true);
        }
        s.normalize();
        t.normalize();
    }
    string vector3fToString(Vector3f v)
    {
        string info = "";
        ostringstream buffer(info);
        buffer << "(x=" << v.x() << ", y=" << v.y() << ", z=" << v.z() << ")";
        return buffer.str();
    }
    Vector3f sameSide(Vector3f n, Vector3f w)
    {
        return n.dot(w) > 0 ? n : -n;
    }
}
