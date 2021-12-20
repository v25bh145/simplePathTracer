//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_LAMBERTIANREFLECTION_H
#define SIMPLEPATHTRACER_LAMBERTIANREFLECTION_H

#include "../../default.h"
#include "../bsdf.h"
#include "../../sampling.h"
#include "../../mathHelper.h"

namespace pathTracer {
    class LambertianReflection : public BSDF {
    public:
        Vector3f kd;
        LambertianReflection(Vector3f kd): kd(kd) {
            type = BxDFType::REFLECTION;
        }

        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;

        void deepCopy(BxDF*& bxdf) override;
    };
}


#endif //SIMPLEPATHTRACER_LAMBERTIANREFLECTION_H
