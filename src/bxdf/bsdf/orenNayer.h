//
// Created by v25bh on 2021/12/26.
//

#ifndef SIMPLEPATHTRACER_ORENNAYER_H
#define SIMPLEPATHTRACER_ORENNAYER_H

#include "../../default.h"
#include "../bsdf.h"

namespace pathTracer {
    class OrenNayer : public BSDF {
    public:
        Vector3f R;
        float sigma;

        float A, B;

        OrenNayer(Vector3f R, float sigma) :R(R), sigma(sigma) {
            type = BxDFType::REFLECTION;

            float sigma2 = sigma * sigma;
            A = 1.f - sigma2 / (2 * (sigma2 + 0.33f));
            B = 0.45f * sigma2 / (sigma2 + 0.09f);
        }
        OrenNayer() {
            R = { 0, 0, 0 };
            sigma = 0;

            type = BxDFType::REFLECTION;

            float sigma2 = sigma * sigma;
            A = 1.f - sigma2 / (2 * (sigma2 + 0.33f));
            B = 0.45f * sigma2 / (sigma2 + 0.09f);
        }

        Vector3f sample_f(Interaction* interaction, Ray* wi, float& pdf, int& sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;

        void deepCopy(BxDF*& bxdf) override;
    };
}


#endif //SIMPLEPATHTRACER_ORENNAYER_H
