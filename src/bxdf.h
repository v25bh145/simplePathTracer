//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_BXDF_H
#define SIMPLEPATHTRACER_BXDF_H

#include "default.h"
#include "interaction.h"
#include "ray.h"

namespace pathTracer {

    class Interaction;
    class Ray;

    enum BxDFType {REFLECTION=0b1, TRANSMISSION=0b10, SPECULAR=0b100, BSSRDF=0b1000};
    class BxDF {
    public:
        unsigned type;
        bool hasType(unsigned type);
        /// \param interaction input: wo, n
        /// \param wi output
        /// \param pdf output
        /// \param sampleType output
        /// \return Vector3f color(percentage)
        /// sample wi with transforming wo from n given to n=(0, 0, 1) and transform this wi from n=(0, 0, 1) to n given
        virtual Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) = 0;
        /// \param interaction input: wo, n
        /// \param wi input
        /// \param pdf output
        /// \return Vector3f color(percentage)
        /// given sampled wi and calculate pdf & f
        virtual Vector3f f(Interaction* interaction, Ray* wi, float& pdf) = 0;
    };
}


#endif //SIMPLEPATHTRACER_BXDF_H
