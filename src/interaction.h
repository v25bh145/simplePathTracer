//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_INTERACTION_H
#define SIMPLEPATHTRACER_INTERACTION_H

#include "default.h"
#include "bxdf.h"
#include "geometry.h"
#include "ray.h"
#include "mathHelper.h"

namespace pathTracer{

    class BxDF;
    class Ray;
    class Geometry;

    class Interaction {
    public:
        Interaction() {};
        Interaction(Ray *ray):ray(ray) {};
        Geometry *geometry;
        Ray *ray;
        Vector3f p;
        Vector3f normal;
        Vector3f normalShading;
        float u;
        float v;
        float dudp;
        float dvdp;
        float du2dp;
        float dv2dp;
        float dndp;
        //BxDF *bxdf;
        string toString();
    };
}


#endif //SIMPLEPATHTRACER_INTERACTION_H
