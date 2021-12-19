//
// Created by v25bh on 2021/11/23.
//

#include "specularReflection.h"

namespace pathTracer {

    Vector3f SpecularReflection::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        Vector3f n = interaction->normal;
        Ray *wo = interaction->ray;

        *wi = Ray(interaction->p, 2 * (abs(n.dot(wo->direction))) * n + wo->direction, 0);
        // TODO:  1 OR cos(theta) / pi?
        //cout << "TEST: wi.dir=" << vector3fToString(wi->direction) << ", -wo.dir=" << vector3fToString(-wo->direction) << endl;
        //cout << "wi.dir-wo.dir=" << vector3fToString(wi->direction - wo->direction) << ", n=" << vector3fToString(n) << endl;
        pdf = abs(interaction->normal.dot(wi->direction));;
        sampleType = type;
        return ks;
    }

    Vector3f SpecularReflection::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        pdf = abs(interaction->normal.dot(wi->direction));;
        return ks;
    }

}