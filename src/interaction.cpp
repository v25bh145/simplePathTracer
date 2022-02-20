//
// Created by v25bh on 2021/11/23.
//

#include "interaction.h"

namespace pathTracer {
    void Interaction::genRayDifferential(float pixelSize)
    {
        this->rayDifferential = new Ray(this->ray);
        this->rayDifferential->origin = { this->ray->origin.x() + pixelSize, this->ray->origin.y(), this->ray->origin.z() };
    }
    void Interaction::deleteRayDifferential()
    {
        if (rayDifferential == nullptr) return;
        delete rayDifferential;
    }
    string Interaction::toString() {
        string info = "";
        ostringstream buffer(info);
        buffer << "p " << vector3fToString(p) << ", " << "n " << vector3fToString(normal) << ", ";
        buffer << "u=" << u << ", v=" << v << endl;
        return buffer.str();
    }
}
