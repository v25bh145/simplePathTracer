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
        float outsideEta;
        float insideEta;
        SpecularTransmission(Vector3f ks, float outsideEta, float insideEta):ks(ks), outsideEta(outsideEta), insideEta(insideEta) { 
            type = BxDFType::SPECULAR | BxDFType::TRANSMISSION; 
        }
        SpecularTransmission() {
            ks = { 0, 0, 0 };
            outsideEta = insideEta = 0;
            type = BxDFType::SPECULAR | BxDFType::TRANSMISSION;
        }

        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;

        void deepCopy(BxDF*& bxdf) override;
    };
}

#endif //SIMPLEPATHTRACER_SPECULARTRANSMISSION_H
