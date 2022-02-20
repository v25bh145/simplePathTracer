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
            rayDifferential = nullptr;
            pixelSizeProjected = 0.f;
        };
        Interaction(Ray *ray):ray(ray) {
            rayDifferential = nullptr;
            geometry = nullptr;
            pixelSizeProjected = 0.f;
        };
        Geometry *geometry;
        Ray *ray;
        Ray *rayDifferential;
        Vector3f p;
        Vector3f normal;
        Vector3f normalShading;
        float time;
        float u;
        float v;
        // RTCu & RTCv are not the coordinate in texture, but parameters in RTC inner core
        float RTCu;
        float RTCv;
        //TODO
        // 投影过去之后像素不再是正方形，甚至两边不再是直角，所以要创建两个微分光线，一个右边像素一个下边像素
        //Vector2f pixelSizeProjected;
        float pixelSizeProjected;
        float dudp;
        float dvdp;
        float du2dp;
        float dv2dp;
        float dndp;
        //BxDF *bxdf;
        void genRayDifferential(float pixelSize);
        void deleteRayDifferential();
        string toString();
    };
}


#endif //SIMPLEPATHTRACER_INTERACTION_H
