//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_SPECULARREFLECTION_H
#define SIMPLEPATHTRACER_SPECULARREFLECTION_H

#include "../../default.h"
#include "../bsdf.h"

namespace pathTracer {
    class SpecularReflection : public BSDF {
    public:
        Vector3f ks;
        SpecularReflection(Vector3f ks):ks(ks){
            type = BxDFType::SPECULAR | BxDFType::REFLECTION;
        }

        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;
    };
}


#endif //SIMPLEPATHTRACER_SPECULARREFLECTION_H
