//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_SPECULARTRANSMISSION_H
#define SIMPLEPATHTRACER_SPECULARTRANSMISSION_H

#include "../../default.h"
#include "../bsdf.h"

namespace pathTracer {
    class SpecularTransmission : public BSDF {
    public:
        Vector3f ks;
        // in / transmit
        float eta;
        SpecularTransmission(Vector3f ks, float eta):ks(ks), eta(eta) { type = BxDFType::SPECULAR | BxDFType::TRANSMISSION; }

        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;
    };
}

#endif //SIMPLEPATHTRACER_SPECULARTRANSMISSION_H
