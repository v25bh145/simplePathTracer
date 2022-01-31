//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_FRESNELSPECULAR_H
#define SIMPLEPATHTRACER_FRESNELSPECULAR_H


#include "../../default.h"
#include "../bsdf.h"
#include "specularReflection.h"
#include "specularTransmission.h"
#include "../../sampling.h"

namespace pathTracer {
    class SpecularReflection;
    class SpecularTransmission;
    class FresnelSpecular : public BSDF {
    public:
        SpecularReflection specularReflection;
        SpecularTransmission specularTransmission;

        FresnelSpecular(Vector3f reflectionKs, Vector3f transmissionKs, float outsideEta, float insideEta) {
            specularReflection = SpecularReflection(reflectionKs);
            specularTransmission = SpecularTransmission(transmissionKs, outsideEta, insideEta);
            type = BxDFType::REFLECTION | BxDFType::TRANSMISSION | BxDFType::SPECULAR;
        }

        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;

        void deepCopy(BxDF*& bxdf) override;
    };
    // dielectric [¾øÔµÌå]
    float FrDielectric(Interaction* interaction);
    float FrIron(Vector3f normal, Vector3f wo_dir);
}

#endif //SIMPLEPATHTRACER_FRESNELSPECULAR_H
