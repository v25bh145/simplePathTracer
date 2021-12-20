//
// Created by v25bh on 2021/11/23.
//

#include "fresnelSpecular.h"

namespace pathTracer {
    Vector3f FresnelSpecular::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        RandomGenerator randomGenerator;
        float u = randomGenerator.uniform0To1();
        
        float Fr = FrDielectric(interaction);
        Vector3f bxdfValue;
        if (u > Fr) {
            // transmission
            bxdfValue = specularTransmission.sample_f(interaction, wi, pdf, sampleType);
            pdf = 1 - Fr;
        }
        else {
            // reflection
            bxdfValue = specularReflection.sample_f(interaction, wi, pdf, sampleType);
            pdf = Fr;
        }
        return bxdfValue;
    }
    Vector3f FresnelSpecular::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        RandomGenerator randomGenerator;
        float u = randomGenerator.uniform0To1();

        float Fr = FrDielectric(interaction);
        Vector3f bxdfValue;
        if (u > Fr) {
            // transmission
            bxdfValue = specularTransmission.f(interaction, wi, pdf);
            pdf = 1 - Fr;
        }
        else {
            // reflection
            bxdfValue = specularReflection.f(interaction, wi, pdf);
            pdf = Fr;
        }
        return bxdfValue;
    }

    void FresnelSpecular::deepCopy(BxDF*& bxdf)
    {
        bxdf = new FresnelSpecular(this->specularReflection.ks, this->specularTransmission.ks, this->specularTransmission.outsideEta, this->specularTransmission.insideEta);
    }

    const float F0 = 0.04f;
    // schlick approximation
    float FrDielectric(Interaction* interaction)
    {
        float costheta = abs(interaction->normal.dot(interaction->ray->direction));
        return F0 + (1.f - F0) * pow(1.f - costheta, 5);
    }
}