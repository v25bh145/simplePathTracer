//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_FRESNELSPECULAR_H
#define SIMPLEPATHTRACER_FRESNELSPECULAR_H


#include "../../default.h"
#include "../bsdf.h"
#include "specularReflection.h"
#include "specularTransmission.h"

namespace pathTracer {
    class SpecularReflection;
    class SpecularTransmission;
    class FresnelSpecular : public BSDF {
    public:
        SpecularReflection specularReflection;
        SpecularTransmission speccularTransmission;
        float eta;

        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;
    };
}

#endif //SIMPLEPATHTRACER_FRESNELSPECULAR_H
