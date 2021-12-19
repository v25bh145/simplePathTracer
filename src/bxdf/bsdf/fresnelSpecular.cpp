//
// Created by v25bh on 2021/11/23.
//

#include "fresnelSpecular.h"

namespace pathTracer {
    Vector3f FresnelSpecular::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        return {0, 0, 0};
    }
    Vector3f FresnelSpecular::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        return Vector3f();
    }
}