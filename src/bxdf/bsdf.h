//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_BSDF_H
#define SIMPLEPATHTRACER_BSDF_H

#include "../default.h"
#include "../bxdf.h"

namespace pathTracer {
    class BSDF : public BxDF {
    public:
        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override = 0;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override = 0;
        virtual void deepCopy(BxDF*& bxdf) override = 0;
    };
}

#endif //SIMPLEPATHTRACER_BSDF_H
