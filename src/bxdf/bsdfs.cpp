//
// Created by v25bh on 2021/11/23.
//

#include "bsdfs.h"

namespace pathTracer {

    Vector3f BSDFs::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        return Eigen::Vector3f();
    }
    Vector3f BSDFs::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        return Vector3f();
    }
}