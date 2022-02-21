//
// Created by v25bh on 2021/11/23.
//

#include "interaction.h"

namespace pathTracer {
    string Interaction::toString() {
        string info = "";
        ostringstream buffer(info);
        buffer << "p " << vector3fToString(p) << ", " << "n " << vector3fToString(normal) << ", ";
        buffer << "u=" << u << ", v=" << v << endl;
        return buffer.str();
    }
}
