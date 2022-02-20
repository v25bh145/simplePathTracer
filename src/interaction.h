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
        //Ray *rayDifferential;
        Vector3f p;
        Vector3f normal;
        Vector3f normalShading;
        float time;
        float u;
        float v;
        // RTCu & RTCv are not the coordinate in texture, but parameters in RTC inner core
        float RTCu;
        float RTCv;
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
