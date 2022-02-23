//
// Created by v25bh on 2021/11/23.
//

#include "interaction.h"

namespace pathTracer {
    Vector2f Interaction::getDuDxy()
    {
        return { this->dudx, this->dudy };
    }
    Vector2f Interaction::getDvDxy()
    {
        return { this->dvdx, this->dvdy };
    }
    string Interaction::toString() {
        string info = "";
        ostringstream buffer(info);
        buffer << "p " << vector3fToString(p) << ", " << "n " << vector3fToString(normal) << ", ";
        buffer << "u=" << u << ", v=" << v << endl;
        return buffer.str();
    }
}
