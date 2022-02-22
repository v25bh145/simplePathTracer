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
        Interaction() {
            geometry = nullptr;
            ray = nullptr;
        };
        Interaction(Ray *ray):ray(ray) {
            geometry = nullptr;
        };
        Geometry *geometry;
        Ray *ray;
        Vector3f p;
        Vector3f normal;
        Vector3f normalShading;
        float time;
        // bartcentric
        float u;
        float v;
        // barycentric's differential
        Vector3f dpdu, dpdv;
        // to calculate(du = {dudx, dudy}, dv = {dvdx, dvdy})
        Vector3f dpdx, dpdy;
        float dudx, dudy, dvdx, dvdy;
        // RTCu & RTCv are not the coordinate in texture, but parameters in RTC inner core
        float RTCu;
        float RTCv;
        //BxDF *bxdf;
        string toString();
    };
}


#endif //SIMPLEPATHTRACER_INTERACTION_H
