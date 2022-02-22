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
            cout << "parameter n passed to orthogonal() is {0.f, 0.f, 0.f}" << endl;
            assert(false);
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
    string vector2fToString(Vector2f v)
    {
        string info = "";
        ostringstream buffer(info);
        buffer << "(x=" << v.x() << ", y=" << v.y() << ")";
        return buffer.str();
    }
    Vector3f sameSide(Vector3f n, Vector3f w)
    {
        return n.dot(w) > 0 ? n : -n;
    }
    bool vector3fEqualTo0(Vector3f vector3f)
    {
        return (vector3f.x() == 0 && vector3f.y() == 0 && vector3f.z() == 0);
    }
    float triangleArea(Vector3f p1, Vector3f p2, Vector3f p3)
    {
        float e1 = (p1 - p2).norm(),
            e2 = (p2 - p3).norm(),
            e3 = (p1 - p3).norm();
        float pt = (e1 + e2 + e3) / 2.f;
        return sqrt(pt * (pt - e1) * (pt - e2) * (pt - e3));
    }
    int clampi(int v, int a, int b)
    {
        v = v < a ? a : (v > b ? b : v);
        return v;
    }
    float clampf(float v, float a, float b)
    {
        v = v < a ? a : (v > b ? b : v);
        return v;
    }
    bool binarySystemOfLinearEquations(Matrix2f A, Vector2f B, Vector2f &X)
    {
        X = { 0.f, 0.f };
        float det = A.determinant();
        if (abs(det) < EPSILON) {
            return false;
        }
        Matrix2f invA = A.inverse();
        X = invA * B;
        return true;
    }
    bool ternarySystemOfLinearEquations(Matrix3f A, Vector3f B, Vector3f& X)
    {
        X = { 0.f, 0.f, 0.f };
        float det = A.determinant();
        if (abs(det) < EPSILON) {
            return false;
        }
        Matrix3f invA = A.inverse();
        X = invA * B;
        return true;
    }
}
